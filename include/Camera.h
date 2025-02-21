#pragma once

#include "shader.h"
#include "Window.h"

class Camera
{
public:
    Window &window;
    void processKeyboardInput(GLFWwindow *window);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    glm::mat4 view;

    Camera(Window &window);
    ~Camera() = default;

    inline void teleport(float x, float y, float z) { cameraPos = glm::vec3(x, y, z); }
    inline glm::vec3 getPlayerPos() { return cameraPos; }

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
