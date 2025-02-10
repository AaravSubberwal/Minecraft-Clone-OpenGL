/****************************************************************************************************************************
Author: Aarav Subberwal
Date:

Cloning Minecraft using OpenGL 4.6, GLAD, GLFW and C++
Work In Progress
currently no way to build except using vscode. I'll add CMake later.
******************************************************************************************************************************/
#include <iostream>
#include <string>
#include <map>
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        float vertices[] = {
            // Positions          // Texture Coords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Back face
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Front face
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Left face
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Right face
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom face
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // Top face
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,   // Back face
            4, 5, 6, 6, 7, 4,   // Front face
            8, 9, 10, 10, 11, 8, // Left face
            12, 13, 14, 14, 15, 12, // Right face
            16, 17, 18, 18, 19, 16, // Bottom face
            20, 21, 22, 22, 23, 20  // Top face
        };
        
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(2);
        va.addBuffer(vb, layout);
        IndexBuffer ib(indices, 36);

        Shader myshader("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/vertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/fragmentShader.glsl");
        Renderer renderer;

        Texture grass("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/dirt.jpg");
        grass.bind(0);
        myshader.setUniform1i("u_grass", 0);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f), // Camera position
            glm::vec3(0.0f, 0.0f, 0.0f), // Look at the origin
            glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
        );

        glm::mat4 model = glm::mat4(1.0f);
        float angle;

        myshader.setUniformMatrix4fv("u_model", model);
        myshader.setUniformMatrix4fv("u_view", view);
        myshader.setUniformMatrix4fv("u_projection", projection);

        while (!glfwWindowShouldClose(window))
        {
            renderer.clear();
            processInput(window);

            angle = (float)glfwGetTime() * glm::radians(50.0f);
            model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.5f, 1.0f, 0.0f));
            myshader.setUniformMatrix4fv("u_model", model);

            renderer.draw(ib, va, myshader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}