#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0);
    fragPos = worldPosition.xyz;
    gl_Position = projection * view * worldPosition;
}
