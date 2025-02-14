#pragma once

#include "shader.h"
#include <GLFW/glfw3.h>

#define WIN_WIDTH 2560
#define WIN_HEIGHT 1600

class Camera
{
public:
    void processKeyboardInput(GLFWwindow *window);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    glm::mat4 view;
    Camera();

private:
    // Camera variables
    glm::vec3 cameraPos;   // Initial camera position
    glm::vec3 cameraFront; // Direction the camera is facing
    glm::vec3 cameraUp;

    // Mouse variables
    float yaw; // Yaw is initialized to -90.0 degrees to look along the negative z-axis
    float pitch;
    float lastX;
    float lastY;
    bool firstMouse;

    // Timing variables
    float deltaTime; // Time between current and last frame
    float lastFrame;
    float currentFrame;
};
