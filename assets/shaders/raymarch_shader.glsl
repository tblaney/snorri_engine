#version 430
layout(local_size_x = 16, local_size_y = 16) in;

struct SurfaceData {
    vec3 position;
};
layout(std430, binding = 0) buffer Surfaces {
    SurfaceData surfaces[];
};

layout(rgba32f, binding = 1) uniform image2D destTex;

struct Ray {
    vec3 origin;
    vec3 direction;
};
struct Surface {
    float distanceToSurface;
};

#define SURF_DIST 0.001
#define MAX_STEPS 150
#define MAX_DIST 300.0

uniform mat4 cameraToWorld;
uniform mat4 cameraInverseProjection;
vec3 cameraForward;

vec3 lightDirection;
vec3 lightPosition;
float time;
int numSurfaces;


Ray createRay(vec3 origin, vec3 direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
}
Ray createCameraRay(vec2 uv) {
    vec3 origin = (cameraToWorld*vec4(0, 0, 0, 1)).xyz;

    vec3 direction = (cameraInverseProjection*vec4(uv, -1, 0)).xyz;
    direction = (cameraToWorld*vec4(direction, 0)).xyz;
    direction = normalize(direction);

    return createRay(origin, direction);
}
float sdSphere(vec3 p, vec3 center, float radius) {
    return length(p - center) - radius;
}
float getSurfaceDistance(vec3 p, SurfaceData surface) {
    float d1 = sdSphere(p, surface.position, 0.5);
    return d1;

    return MAX_DIST;
}
Surface getSurface(vec3 position) {
    Surface surface = {MAX_DIST};
    surface.distanceToSurface = getSurfaceDistance(position, surfaces[0]);
    return surface;
}
Surface rayMarch(Ray ray) {
    float distanceToScene = 0;
    Surface closestSurface = {MAX_DIST};
    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 step = ray.origin + ray.direction * distanceToScene;
        closestSurface = getSurface(step);

        if (closestSurface.distanceToSurface < SURF_DIST) {
            closestSurface.distanceToSurface = distanceToScene + closestSurface.distanceToSurface;
            break;
        }
        if (distanceToScene >= MAX_DIST) {
            closestSurface.distanceToSurface = MAX_DIST;
            break;
        }
        distanceToScene += closestSurface.distanceToSurface;
    }
    closestSurface.distanceToSurface = distanceToScene;

    return closestSurface;
}

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    ivec2 size = imageSize(destTex);
    uint width = uint(size.x);
    uint height = uint(size.y);
    vec2 uv = (vec2(id) / vec2(width, height));

    vec4 color = vec4(1.0,0.0,0.0,1.0);

    Ray ray = createCameraRay(uv);

    //color.x = ray.origin.x;
    color.x = uv.x;
    //color.y = clamp(ray.origin.x,0,1);
    //color.z = ray.origin.z;

    imageStore(destTex, id, color); 
}
