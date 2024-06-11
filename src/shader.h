#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    void use() const;

private:
    GLuint compileShader(GLenum type, const std::string& source);
    std::string readFile(const std::string& filePath);
    GLuint program;
};

#endif // SHADER_H
