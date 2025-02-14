#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "textures.h"
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
        const float atlasSize = 16.0f;           // 16x16 grid (each cell is 16x16 pixels)
        const float cellSize = 1.0f / atlasSize; // Each texture takes 1/16th of the atlas (0.0625)

        const int col = 3, row = 15 - (14); // Change these to select different textures
        const float u = col * cellSize;
        const float v = row * cellSize;

        float vertices[] = {
            // Positions          // Texture Coords (Atlas-based)
            0.0f, 0.0f, 0.0f, u, v + cellSize, // Back face
            1.0f, 0.0f, 0.0f, u + cellSize, v + cellSize,
            1.0f, 1.0f, 0.0f, u + cellSize, v,
            0.0f, 1.0f, 0.0f, u, v,

            0.0f, 0.0f, 1.0f, u, v + cellSize, // Front face
            1.0f, 0.0f, 1.0f, u + cellSize, v + cellSize,
            1.0f, 1.0f, 1.0f, u + cellSize, v,
            0.0f, 1.0f, 1.0f, u, v,

            0.0f, 0.0f, 0.0f, u, v + cellSize, // Left face
            0.0f, 0.0f, 1.0f, u + cellSize, v + cellSize,
            0.0f, 1.0f, 1.0f, u + cellSize, v,
            0.0f, 1.0f, 0.0f, u, v,

            1.0f, 0.0f, 0.0f, u, v + cellSize, // Right face
            1.0f, 0.0f, 1.0f, u + cellSize, v + cellSize,
            1.0f, 1.0f, 1.0f, u + cellSize, v,
            1.0f, 1.0f, 0.0f, u, v,

            0.0f, 0.0f, 0.0f, u, v + cellSize, // Bottom face
            1.0f, 0.0f, 0.0f, u + cellSize, v + cellSize,
            1.0f, 0.0f, 1.0f, u + cellSize, v,
            0.0f, 0.0f, 1.0f, u, v,

            0.0f, 1.0f, 0.0f, u, v + cellSize, // Top face
            1.0f, 1.0f, 0.0f, u + cellSize, v + cellSize,
            1.0f, 1.0f, 1.0f, u + cellSize, v,
            0.0f, 1.0f, 1.0f, u, v};

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,       // Back face
            4, 5, 6, 6, 7, 4,       // Front face
            8, 9, 10, 10, 11, 8,    // Left face
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

        Shader shader("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/vertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/fragmentShader.glsl");
        Renderer renderer;

        Texture atlas("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/terrain.png");
        atlas.bind();
        shader.setUniform1i("u_atlas", 0);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGTH, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));

        shader.setUniformMatrix4fv("u_model", model);
        shader.setUniformMatrix4fv("u_projection", projection);

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.processKeyboardInput(window);
            shader.setUniformMatrix4fv("u_view", camera.view);

            renderer.draw(ib, va, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}