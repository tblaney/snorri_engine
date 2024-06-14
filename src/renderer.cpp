#include "renderer.h"
#include <stb_image.h>
#include <iostream>
#include <glm/glm.hpp>

// Constructor
Renderer::Renderer(const std::string& vertexPath, const std::string& fragmentPath, const std::string& computePath) 
    : shader(vertexPath, fragmentPath), compute(computePath) {

    float vertices[] = {
        // positions       // texture coords
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    createSolidColorTexture(640, 360, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Renderer::createSolidColorTexture(int width, int height, const glm::vec3& color) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Prepare color data for the texture
    unsigned char* data = new unsigned char[width * height * 3];
    for (int i = 0; i < width * height; ++i) {
        data[i * 3 + 0] = static_cast<unsigned char>(color.r * 255);
        data[i * 3 + 1] = static_cast<unsigned char>(color.g * 255);
        data[i * 3 + 2] = static_cast<unsigned char>(color.b * 255);
    }

    // Create texture
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
}

// Render function
void Renderer::render(glm::mat4 view, glm::mat4 projection) {
    glClear(GL_COLOR_BUFFER_BIT);

    compute.setTexture(texture, 0);  // Binding the texture to image unit 0
    compute.use();
    compute.dispatch(640, 360, 1);

    shader.use();
    
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindTexture(GL_TEXTURE_2D, texture);

    shader.setInt("texture1", 0); // Set the texture uniform to use texture unit 0

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}