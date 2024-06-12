#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose() const;
    void clear() const;
    void display() const;
    void pollEvents() const;
    GLFWwindow* getGLFWwindow() const; // Add this line

private:
    GLFWwindow* window;
};

#endif // WINDOW_H
