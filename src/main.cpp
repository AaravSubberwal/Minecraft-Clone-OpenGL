#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "textures.h"

#define WIN_WIDTH 2560
#define WIN_HEIGTH 1600
#define CELLSIZE 16
using namespace std;

// Camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);    // Initial camera position
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Direction the camera is facing
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);     // Up direction

// Mouse variables
float yaw = -90.0f; // Yaw is initialized to -90.0 degrees to look along the negative z-axis
float pitch = 0.0f;
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGTH / 2.0f;
bool firstMouse = true;

// Timing variables
float deltaTime = 0.0f; // Time between current and last frame
float lastFrame = 0.0f;

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

    float cameraSpeed = 5.0f * deltaTime; // Adjust speed based on time

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraPos += 2.0f * cameraSpeed * cameraFront;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    // Move up
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraUp;
    }
    // Move down
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraUp;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // Adjust sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to avoid flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Update cameraFront based on yaw and pitch
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
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
    glfwSetCursorPosCallback(window, mouse_callback);            // Set mouse callback
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide and capture cursor

    enableDebugging();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        ;
        const float atlasSize = 16.0f;           // 16x16 grid (each cell is 16x16 pixels)
        const float cellSize = 1.0f / atlasSize; // Each texture takes 1/16th of the atlas (0.0625)

        const int col = 4, row = 15; // Change these to select different textures
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

        Shader myshader("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/vertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/fragmentShader.glsl");
        Renderer renderer;

        Texture grass("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/terrain.png");
        grass.bind();
        myshader.setUniform1i("u_grass", 0);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGTH, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);

        myshader.setUniformMatrix4fv("u_model", model);
        myshader.setUniformMatrix4fv("u_projection", projection);

        float currentFrame = 0;
        while (!glfwWindowShouldClose(window))
        {
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            renderer.clear();
            processInput(window);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            myshader.setUniformMatrix4fv("u_view", view, GL_FALSE);

            renderer.draw(ib, va, myshader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}