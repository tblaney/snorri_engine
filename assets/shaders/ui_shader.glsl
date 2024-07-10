layout(local_size_x = 8, local_size_y = 8) in;

struct UiData {
    vec4 position;
    vec4 rotation;
    vec4 scale;
    vec4 diffuse;
};
layout(std430, binding = 0) buffer Uis {
    UiData uis[];
};

struct ResultData {
    vec3 worldPosition;
    float pad;
};
layout(std430, binding = 2) buffer Results {
    ResultData results[];
};

layout(rgba32f, binding = 1) uniform image2D destTex;

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    ivec2 size = imageSize(destTex);
    uint width = uint(size.x);
    uint height = uint(size.y);
    vec2 uv = (vec2(id) / vec2(width, height)) * 2.0 - 1.0;

    imageStore(destTex, id, vec4(0.0,0.0,0.0,0.0)); 
}
