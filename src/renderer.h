#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

class Renderer {
public:
    Renderer();
    ~Renderer();
    void render() const;

private:
    GLuint vao, vbo, ebo;
};

#endif // RENDERER_H
