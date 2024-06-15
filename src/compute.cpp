#include "compute.h"
#include "surface/surface.h"
#include <glm/gtc/type_ptr.hpp>

ComputeShader::ComputeShader() {
        
}

void ComputeShader::setShaderPath(const std::string& filePath) {
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

void ComputeShader::setupResultBuffer(size_t numElements) {
    glGenBuffers(1, &ssboResult);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboResult);
    glBufferData(GL_SHADER_STORAGE_BUFFER, numElements * sizeof(ResultData), nullptr, GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboResult);  // Assuming binding = 2 for results
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

std::vector<ResultData> ComputeShader::retrieveResults(size_t numElements) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboResult);
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);  // Ensure all writes are finished

    void* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    if (ptr) {
        std::vector<ResultData> results(numElements);
        memcpy(results.data(), ptr, numElements * sizeof(ResultData));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        return results;
    } else {
        throw std::runtime_error("Failed to map buffer");
    }
}

void ComputeShader::printResults(std::vector<ResultData>& results) {
    std::cout << "Retrieved Results:" << std::endl;
    for (const auto& result : results) {
        std::cout << "Position: " << result.worldPosition.x << ", " << result.worldPosition.y << ", " << result.worldPosition.z << std::endl;
    }
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

void ComputeShader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ComputeShader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void ComputeShader::setBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), static_cast<int>(value));
}

void ComputeShader::setMat4(const std::string& name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void ComputeShader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
}

void ComputeShader::createBuffer(GLuint* buffer, GLsizeiptr size, const void* data, GLenum usage) {
    glGenBuffers(1, buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, *buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ComputeShader::setupSurfaceBuffer(const std::vector<SurfaceData>& surfaces) {
    createBuffer(&ssbo, surfaces.size() * sizeof(SurfaceData), surfaces.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); // Bind to binding point 0
}
