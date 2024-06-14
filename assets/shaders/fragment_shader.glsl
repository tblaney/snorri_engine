#version 330 core

struct VertexOut {
    vec3 fragPos;
    vec2 TexCoord;
};

in vec2 TexCoord; // Declare the input variable using the struct
out vec4 FragColor;
uniform sampler2D texture1;

void main() {
    //FragColor = texture(texture1, TexCoord);
    FragColor = vec4(0.0,1.0,0.0,1.0);
}
