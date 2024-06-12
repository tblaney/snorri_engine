#version 330 core

out vec4 FragColor;
in vec3 fragPos;

float sphereSDF(vec3 p, float r) {
    return length(p) - r;
}

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Inside the sphere (white color)
    /*
    float d = sphereSDF(fragPos, 0.5); // Sphere with radius 0.5
    if (d < 0.0) {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // Inside the sphere (white color)
    } else {
        discard; // Outside the sphere
    }
    */
}
