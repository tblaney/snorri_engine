#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec3 fragPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    fragPos = position;
    gl_Position = projection * view * vec4(position, 1.0);
}
