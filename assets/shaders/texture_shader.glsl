#version 430 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(rgba32f, binding = 0) uniform image2D destTex;

void main() {
    ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
    imageStore(destTex, storePos, vec4(0.5, 0.2, 0.8, 1.0)); // Set to a purple color
}
