#include "compute.h"

ComputeShader::ComputeShader(const std::string& filePath) {
    std::string shaderCode = readShaderFile(filePath);
    const char* shaderSource = shaderCode.c_str();

    // Create the shader
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, "COMPUTE");

    // Create the program and link it
    programID = glCreateProgram();
    glAttachShader(programID, shader);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");

    // Delete the shader as it's linked into our program now and no longer necessary
    glDeleteShader(shader);
}

ComputeShader::~ComputeShader() {
    glDeleteProgram(programID);
}

void ComputeShader::use() {
    glUseProgram(programID);
}

void ComputeShader::dispatch(int width, int height, int depth) {
    glDispatchCompute((GLuint)ceil(width / 16.0f), (GLuint)ceil(height / 16.0f), depth);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShader::setTexture(GLuint textureID, GLuint unit) {
    glBindImageTexture(unit, textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

std::string ComputeShader::readShaderFile(const std::string& filePath) {
    std::ifstream shaderFile;
    std::stringstream shaderStream;

    // Ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // Open files
        shaderFile.open(filePath);
        shaderStream << shaderFile.rdbuf(); // Read file's buffer contents into streams
        shaderFile.close(); // Close file handlers
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
    }
    return shaderStream.str();
}

void ComputeShader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
