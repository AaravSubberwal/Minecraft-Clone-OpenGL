#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "textures.h"
#include "Minecraft.h"
#include "Camera.h"

#define WIN_WIDTH 2560
#define WIN_HEIGTH 1600
using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
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
    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGTH, "Minecraft Spectator Mode", NULL, NULL);
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

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGTH);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide and capture cursor
    Camera camera;
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, Camera::mouse_callback);

    enableDebugging();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    {
        Shader shader("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/vertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/fragmentShader.glsl");
        World world(shader, camera);

        while (!glfwWindowShouldClose(window))
        {
            camera.processKeyboardInput(window);
            shader.setUniformMatrix4fv("u_view", camera.view);

            world.render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}