#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "shader.h"
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           float yaw = -90.0f, float pitch = 0.0f);

    glm::mat4 getViewMatrix() const;
    void processKeyboard(GLFWwindow* window, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;

private:
    void updateCameraVectors();
};

#endif // CAMERA_H