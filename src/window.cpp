#include "window.h"
#include <iostream>

GLFWwindow* Window::window = nullptr; // Initialize static member

Window::Window(int width, int height, const std::string& title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window\n";
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        exit(EXIT_FAILURE);
    }
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr; // Reset static member
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::display() const {
    glfwSwapBuffers(window);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

GLFWwindow* Window::getGLFWwindow() { // Implement this method
    return window;
}
