#include "renderer.h"
#include <stb_image.h>
#include <iostream>

// Constructor
Renderer::Renderer(const Point& rendererPoint, const Camera& cam, const std::string& vertexPath, const std::string& fragmentPath) 
    : Object(rendererPoint), shader(vertexPath, fragmentPath), 
      camera(cam) {

    updateVerticesBasedOnCamera();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // set to 0 index
    
    // tex attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // set to 1 index

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    loadTexture("../assets/images/test.png");
}

void Renderer::update() {
    // Implementation of the update method
    // You can add any updates related to the Renderer here
}

void Renderer::updateVerticesBasedOnCamera() {
    float aspectRatio = camera.aspectRatio;
    float halfHeight = 1.0f;
    float halfWidth = aspectRatio * halfHeight;

    float newVertices[] = {
        // positions       // texture coords
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f
    };

    // Update the vertices array in the class
    for (int i = 0; i < 20; ++i) {
        vertices[i] = newVertices[i];
    }
}

void Renderer::updateVertices() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Load texture
void Renderer::loadTexture(const char* path) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

// Render function
void Renderer::render() const {
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    
    glm::mat4 view = camera.viewMatrix;
    glm::mat4 projection = camera.projectionMatrix;

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", getPoint().getModelMatrix()); // Use the model matrix from the Point

    glBindTexture(GL_TEXTURE_2D, texture);

    shader.setInt("texture1", 0); // Set the texture uniform to use texture unit 0

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
