#version 430
layout(local_size_x = 16, local_size_y = 16) in;

struct SurfaceData {
    vec3 position;
};
layout(std430, binding = 0) buffer Surfaces {
    SurfaceData surfaces[];
};

struct ResultData {
    vec3 worldPosition;
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

    vec3 direction = (cameraInverseProjection*vec4(uv, -1, 0)).xyz;
    direction = (cameraToWorld*vec4(direction, 0)).xyz;
    direction = normalize(direction);

    return createRay(origin, direction);
}
void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    ivec2 size = imageSize(destTex);
    vec2 uv = (vec2(id) + 0.5) / vec2(size); // +0.5 for pixel center
    vec2 ndc = (uv * 2.0) - 1.0;

    vec4 clipSpacePosition = vec4(ndc, -1.0, 1.0);
    vec3 visualizedPosition = vec3((clipSpacePosition.x + 1.0) / 2.0, (clipSpacePosition.y + 1.0) / 2.0, 0.0);
    vec4 eyeSpacePosition = cameraInverseProjection * clipSpacePosition;
    eyeSpacePosition /= eyeSpacePosition.w;
    vec4 worldSpacePosition = cameraToWorld * eyeSpacePosition;
    vec3 worldPosition = worldSpacePosition.xyz;

    Ray cameraRay = createCameraRay(uv);

    results[id.y * size.x + id.x].worldPosition = worldPosition;


    vec4 color = vec4(1.0,0.0,0.0,1.0);
    color.x = uv.x;
    //color.x = clamp(length(clipSpacePosition), 0, 1);

    imageStore(destTex, id, vec4(worldPosition.xyz,1.0)); 
}
