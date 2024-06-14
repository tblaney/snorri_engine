#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

struct VertexOut {
    vec3 fragPos;
    vec2 TexCoord;
};
out vec2 TexCoord; // Declare the output variable using the struct

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    //vec4 worldPosition = model * vec4(position, 1.0);
    //gl_Position = projection * view * worldPosition;
    //outData.fragPos = worldPosition.xyz; // Set fragPos in the output data struct
    TexCoord = texCoord; // Set TexCoord in the output data struct
}
