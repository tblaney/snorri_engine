#include "renderer.h"
#include "log.h"
#include <iostream>

// Constructor
Renderer::Renderer(
    const std::string& vertexPath, 
    const std::string& fragmentPath, 
    const std::string& computePath) 
    : shader(vertexPath, fragmentPath), 
        computePath(computePath) {

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

    loadTexture();
}

void Renderer::loadAndRunComputeShader() {
    // Load and compile the compute shader
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    std::string computeShaderCode = Shader::readFile(computePath);
    const char* cShaderCode = computeShaderCode.c_str();
    glShaderSource(computeShader, 1, &cShaderCode, NULL);
    glCompileShader(computeShader);
    checkShaderCompilation(computeShader);

    // Create a program and attach the shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, computeShader);
    glLinkProgram(shaderProgram);
    checkProgramLinking(shaderProgram);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Bind texture to image unit
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    
    // Dispatch the compute shader
    glDispatchCompute((GLuint)(1024 / 16), (GLuint)(1024 / 16), 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); // Ensure completion before using the texture

    // Cleanup
    glDeleteShader(computeShader);
    glUseProgram(0);

    Log::console("renderer setup!");
}

void Renderer::checkShaderCompilation(GLuint shader) {
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::string errorLog(maxLength, ' ');
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manner you deem best.
        std::cerr << "Shader compilation failed: " << errorLog << std::endl;

        // Don't forget to delete the shader as it's useless now.
        glDeleteShader(shader);
    }
}
void Renderer::checkProgramLinking(GLuint program) {
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::string errorLog(maxLength, ' ');
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manner you deem best.
        std::cerr << "Program linking failed: " << errorLog << std::endl;

        // We don't need the program anymore.
        glDeleteProgram(program);
    }
}


// Load texture
void Renderer::loadTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping and filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Define an empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Load and run the compute shader to set texture values
    loadAndRunComputeShader();
}

// Render function
void Renderer::render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    
    glm::mat4 view = viewMatrix;
    glm::mat4 projection = projectionMatrix;

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindTexture(GL_TEXTURE_2D, texture);

    shader.setInt("texture1", 0); // Set the texture uniform to use texture unit 0

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}