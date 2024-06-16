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
};

#define SURF_DIST 0.001
#define MAX_STEPS 150
#define MAX_DIST 300.0

uniform mat4 cameraToWorld;
uniform mat4 cameraInverseProjection;
uniform vec3 cameraForward;

uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform float time;
uniform int numSurfaces;


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
        s1.diffuse = vec4(
            opSmoothUnion(s1.diffuse.x, s2.diffuse.x, blendStrength),
            opSmoothUnion(s1.diffuse.y, s2.diffuse.y, blendStrength),
            opSmoothUnion(s1.diffuse.z, s2.diffuse.z, blendStrength),
            1.0
        );
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
Surface rayMarch(Ray ray) {
    Surface surface;
    float distanceToSurface = 0.0;
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 pos = ray.origin + ray.direction*distanceToSurface;
        Surface surf = getSurface(pos);
        surface.diffuse = surf.diffuse;
        float d = surf.distanceToSurface;

        if (d < SURF_DIST || distanceToSurface > MAX_DIST) break;

        distanceToSurface += d;
    }
    surface.distanceToSurface = distanceToSurface;
    return surface;
}

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    ivec2 size = imageSize(destTex);
    uint width = uint(size.x);
    uint height = uint(size.y);
    vec2 uv = (vec2(id) / vec2(width, height)) * 2.0 - 1.0;

    Ray ray = createCameraRay(uv);
    Surface closestSurface = rayMarch(ray);

    vec4 color = vec4(0.0,0.0,0.0,1.0);
    if (closestSurface.distanceToSurface < MAX_DIST) {
        color = closestSurface.diffuse;
    }

    ResultData data;
    data.worldPosition = vec3(id.x,id.y,surfaces[0].shapeType);
    results[id.y*size.x+id.x] = data;

    imageStore(destTex, id, vec4(color.xyz,1.0)); 
}
