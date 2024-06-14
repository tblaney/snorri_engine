#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h> // or the appropriate OpenGL header for your system
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;
    void setInt(const std::string& name, int value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    static std::string readFile(const std::string& filePath);

private:
    GLuint program;

    GLuint compileShader(GLenum type, const std::string& source);
};

#endif
