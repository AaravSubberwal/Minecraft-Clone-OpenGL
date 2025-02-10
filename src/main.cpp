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

#include "Renderer.h"
#include "textures.h"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(800, 800, "Aarav", NULL, NULL);
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

    glViewport(0, 0, 800, 800);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    enableDebugging();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // Top-right
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // Top-left
        };

        unsigned int indices[] = {
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
        };

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices), GL_STATIC_DRAW);
        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(2);
        va.addBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        // glm::mat4 proj = glm::ortho()
        Shader myshader("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/vertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/fragmentShader.glsl");
        Renderer renderer;

        Texture grass("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/grass.jpeg");
        grass.bind(0);
        myshader.setUniform1i("u_grass", 0);

        while (!glfwWindowShouldClose(window))
        {
            renderer.clear();
            processInput(window);

            glm::mat4 model = glm::mat4(1.0f);
            float angle = (float)glfwGetTime();                             // Rotate over time
            model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

            myshader.setUniformMatrix4fv("u_model", model, GL_FALSE);

            renderer.draw(ib, va, myshader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}