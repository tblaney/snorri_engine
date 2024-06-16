#version 430
layout(local_size_x = 8, local_size_y = 8) in;

struct SurfaceData {
    vec3 position;
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
float sdSphere(vec3 p, vec3 center, float radius) {
    return length(p - center) - radius;
}
float getSurfaceDistance(vec3 p, SurfaceData surface) {
    float d1 = sdSphere(p, vec3(0.0,0.0,0.0), 0.5);
    return d1;
}
Surface getSurface(vec3 position) {
    Surface surface = {MAX_DIST};
    surface.distanceToSurface = getSurfaceDistance(position, surfaces[0]);
    return surface;
}
float rayMarch(Ray ray) {
    float distanceToSurface = 0.0;
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 pos = ray.origin + ray.direction*distanceToSurface;
        float d = sdSphere(pos, surfaces[0].position, 0.75);

        if (d < SURF_DIST || distanceToSurface > MAX_DIST) break;

        distanceToSurface += d;
    }

    return distanceToSurface;
}

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    ivec2 size = imageSize(destTex);
    uint width = uint(size.x);
    uint height = uint(size.y);
    vec2 uv = (vec2(id) / vec2(width, height)) * 2.0 - 1.0;

    Ray ray = createCameraRay(uv);
    float closestSurface = rayMarch(ray);

    //bool isSurfaceExists = closestSurface.distanceToSurface < MAX_DIST;

    vec4 color = vec4(0.0,1.0,0.0,1.0);
    if (closestSurface < MAX_DIST)
        color = vec4(1.0,0.0,0.0,1.0);

    //results[id.y * size.x + id.x].worldPosition = vec3(id.x,id.y,closestSurface.distanceToSurface);
    //results[id.y * size.x + id.x].worldPosition = vec3(1,1,closestSurface.distanceToSurface);
    ResultData data;
    //data.worldPosition = vec3(1,1,1);
    //data.worldPosition = vec3(id.x,id.y,sdSphere(ray.origin + ray.direction*3, vec3(0,0,0), 1.0));
    data.worldPosition = vec3(id.x,id.y,closestSurface);
    results[id.y*size.x+id.x] = data;

    imageStore(destTex, id, vec4(color.xyz,1.0)); 
}
