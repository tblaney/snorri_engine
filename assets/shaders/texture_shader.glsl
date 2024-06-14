#version 430
layout(local_size_x = 16, local_size_y = 16) in;

layout(rgba32f, binding = 0) uniform image2D destTex;

void main() {
    ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
    imageStore(destTex, storePos, vec4(0.0, 1.0, 0.0, 1.0)); // Set to green
}
