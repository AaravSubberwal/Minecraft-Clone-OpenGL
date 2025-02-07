/****************************************************************************************************************************
Author: Aarav Subberwal
Date:

Cloning Minecraft using OpenGL 4.6, GLAD, GLFW and C++
Work In Progress
currently no way to build except using vscode. I'll add CMake later.
******************************************************************************************************************************/
#include <iostream>
#include <string>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(800, 600, "Aarav", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    {
        float vertices[] = {
            // Position (x, y, z)    Color (r, g, b, a)
            -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f, // Vertex 1 (Red)
             0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f, // Vertex 2 (Green)
             0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f  // Vertex 3 (Blue)
        };
        unsigned int indices[] = {
            0, 1, 2 };

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices), GL_STATIC_DRAW);
        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(4);
        va.addBuffer(vb, layout);
        IndexBuffer ib(indices, 3);

        // glm::mat4 proj = glm::ortho()
        Shader myshader("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/vertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/fragmentShader.glsl");
        myshader.bind();
        myshader.setUniform4f("u_color", 0.0, 1.0, 0.0, 1.0);
        Renderer renderer;

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);
            renderer.backColor(0.0f, 0.0f, 1.0f, 1.0f);

            renderer.draw(ib, va, myshader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}