#pragma once
#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

using namespace std;

class Window
{
private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    GLFWwindow *window;
    const unsigned int WIN_WIDTH = 2560;
    const unsigned int WIN_HEIGTH = 1600;

public:
    inline GLFWwindow *p_GLFWwindow() { return window; }
    inline unsigned int getWidth() { return WIN_WIDTH; }
    inline unsigned int getHeight() { return WIN_HEIGTH; }

    Window();
    ~Window();
};