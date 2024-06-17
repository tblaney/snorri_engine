layout(local_size_x = 8, local_size_y = 8) in;

struct SurfaceData {
    vec4 position;
    vec4 rotation;
    vec4 scale;
    vec4 diffuse;
    int shapeType;
    int blendType;
    float blendStrength;
    int outline;
};
layout(std430, binding = 0) buffer Surfaces {
    SurfaceData surfaces[];
};

struct ResultData {
    vec3 worldPosition;
    float pad;
};
layout(std430, binding = 2) buffer Results {
    ResultData results[];
};

layout(rgba32f, binding = 1) uniform image2D destTex;

struct Ray {
    vec3 origin;
    vec3 direction;
};
struct Surface {
    float distanceToSurface;
    vec4 diffuse;
    float outline;
};

#define SURF_DIST 0.001
#define MAX_STEPS 150
#define MAX_DIST 300.0
#define EPSILON 0.001;
#define SHADOW_BIAS 0.05;

uniform mat4 cameraToWorld;
uniform mat4 cameraInverseProjection;
uniform vec3 cameraForward;

uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform float time;
uniform int numSurfaces;
uniform float outlineWidth;


Ray createRay(vec3 origin, vec3 direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
}
Ray createCameraRay(vec2 uv) {
    vec3 origin = (cameraToWorld*vec4(0, 0, 0, 1)).xyz;

    vec3 direction = (cameraInverseProjection*vec4(uv, 0, 1)).xyz;
    direction = (cameraToWorld*vec4(direction, 0)).xyz;
    direction = normalize(direction);

    return createRay(origin, direction);
}
float getSurfaceDistance(vec3 p, SurfaceData surface) {
    if (surface.shapeType == 0) {
        return sdEllipsoid(p, surface.scale.xyz);
    } else if (surface.shapeType == 1) {
        return sdBox(p, surface.scale.xyz/2.0);
    } else if (surface.shapeType == 2) {
        return sdPlane(p, vec3(0,1,0), 0);
    }
    float d1 = sdSphere(p, 0.5);
    return d1;
}
Surface blend(Surface s1, Surface s2, float blendStrength, int blendMode)
{
    if (blendMode == 0)
    {
        s1.distanceToSurface = opUnion(s1.distanceToSurface, s2.distanceToSurface);
        if (s1.distanceToSurface >= s2.distanceToSurface)
        {
            s1.diffuse = s2.diffuse;
        }
    }
    if (blendMode == 1)
    {
        s1.distanceToSurface = opSmoothUnion(s1.distanceToSurface, s2.distanceToSurface, blendStrength);
        if (s1.distanceToSurface >= s2.distanceToSurface)
        {
            s1.diffuse = s2.diffuse;
        }
    }
    return s1;
}
// Rotation around the X-axis
mat3 rotateX(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(1, 0, 0),
        vec3(0, c, -s),
        vec3(0, s, c)
    );
}
// Rotation around the Y-axis
mat3 rotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
}
// Rotation around the Z-axis
mat3 rotateZ(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, -s, 0),
        vec3(s, c, 0),
        vec3(0, 0, 1)
    );
}
Surface getSurface(vec3 position) {
    Surface surface;
    surface.distanceToSurface = MAX_DIST;
    surface.diffuse = vec4(1,1,1,1);
    for (int i = 0; i < numSurfaces; i++)
    {
        SurfaceData shape = surfaces[i];
        Surface shapeSurf;
        shapeSurf.diffuse = shape.diffuse;
        mat3 rotation = rotateX(-shape.rotation.x) * rotateY(-shape.rotation.y) * rotateZ(-shape.rotation.z);
        vec3 pos = position - shape.position.xyz;
        vec3 transformedPosition = rotation * (position - shape.position.xyz);

        shapeSurf.distanceToSurface = getSurfaceDistance(transformedPosition, shape);
        surface = blend(surface, shapeSurf, shape.blendStrength, shape.blendType);
    }
    return surface;
}
Surface rayMarch(Ray ray, bool isSolidIncrement) {
    Surface surface;
    float lastSDF = MAX_DIST;
    float edge = 0.0;
    float distanceToSurface = 0.0;
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 pos = ray.origin + ray.direction*distanceToSurface;
        if (isSolidIncrement)
        {
            pos = ray.origin + ray.direction*i;
        }
        Surface surf = getSurface(pos);
        surface.diffuse = surf.diffuse;
        float d = surf.distanceToSurface;

        if ((lastSDF < outlineWidth) && (d > lastSDF)) {
            edge = 1.0;
        }

        if (d < SURF_DIST || distanceToSurface > MAX_DIST) break;

        distanceToSurface += d;
        lastSDF = d;

        if (edge > 0.9)
            break;
    }
    surface.distanceToSurface = distanceToSurface;
    surface.outline = edge;
    return surface;
}
vec3 getNormal(vec3 surfPoint)
{
    float epsilon = 0.0001;
    float centerDistance = getSurface(surfPoint).distanceToSurface;
    float xDistance = getSurface(surfPoint + vec3(epsilon, 0, 0)).distanceToSurface;
    float yDistance = getSurface(surfPoint + vec3(0, epsilon, 0)).distanceToSurface;
    float zDistance = getSurface(surfPoint + vec3(0, 0, epsilon)).distanceToSurface;
    vec3 normal = normalize(vec3(xDistance, yDistance, zDistance) - centerDistance);
    return normal;
}
float rampLighting(vec3 normalDir, vec3 lightDir)
{
    // Calculate the Lambertian reflectance
    float lambert = max(dot(normalDir, -lightDir), 0.0);

    // Define the number of steps for the toon shading
    int steps = 4;

    // Quantize the lambert value into discrete steps
    float toon = floor(lambert * steps) / steps;

    return toon;
}
float getHardShadow(Ray ray, float mint, float tmax)
{
    float t = mint;

    for (int i = 0; i < 16; i++)
    {
        vec3 vec = ray.origin + ray.direction * t;
        float h = getSurface(vec).distanceToSurface;

        if (h < 0.001) {
            return 0.0; // Shadowed
        }

        t += h; // Move to the next step along the ray
        if (t > tmax) {
            break; // Exit if the max distance is exceeded
        }
    }

    return 1.0; // Fully lit if no occlusion found
}
float getSoftShadow(Ray ray, float mint, float tmax)
{
    float res = 1.0;
    float t = mint;

    for (int i = 0; i < 16; i++)
    {
        vec3 vec = ray.origin + ray.direction * t;
        float h = getSurface(vec).distanceToSurface;
        res = min(res, 12.0 * h / t);
        t += clamp(h, 0.02, 0.10);
        if (h < 0.001 || t > tmax) break;
    }

    return clamp(res, 0.0, 1.0);
}
vec4 getPixelColor(vec3 lightDir, Surface surface, vec3 surfPoint, vec3 normal)
{
    float diffuseMask = max(0.1, rampLighting(normal, lightDir));
    Ray shadowRay = createRay(surfPoint + normal * SURF_DIST * 10.0, -lightDir);
    //Ray shadowRay = createRay(lightPosition, lightDir);
    diffuseMask *= max(getSoftShadow(shadowRay, 0.02, 8.0), 0.2);
    //diffuseMask *= max(getHardShadow(shadowRay, 0.02, 8.0), 0.1);

    return diffuseMask * surface.diffuse;
}

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    ivec2 size = imageSize(destTex);
    uint width = uint(size.x);
    uint height = uint(size.y);
    vec2 uv = (vec2(id) / vec2(width, height)) * 2.0 - 1.0;

    Ray ray = createCameraRay(uv);
    Surface closestSurface = rayMarch(ray, false);
    vec3 surfPoint = ray.origin + ray.direction * closestSurface.distanceToSurface;

    float outValue = 0.0;

    vec4 color = vec4(1.0,1.0,1.0,1.0);
    if (closestSurface.distanceToSurface < MAX_DIST && closestSurface.outline <= 0.9) {
        //color = closestSurface.diffuse;
        color = getPixelColor(lightDirection.xyz, closestSurface, surfPoint, getNormal(surfPoint));
    } else if (closestSurface.outline > 0.9) {
        color = vec4(0.0,0.0,0.0,1.0);
    }

    ResultData data;
    data.worldPosition = vec3(id.x,id.y,outValue);
    results[id.y*size.x+id.x] = data;

    imageStore(destTex, id, vec4(color.xyz,1.0)); 
}
