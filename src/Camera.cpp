#include "Camera.h"

Camera::Camera() : cameraPos(glm::vec3(0.0f, 5.0f, 0.0f)),
                   cameraFront(glm::vec3(0.0f, 0.0f, 1.0f)),
                   cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
                   yaw(-90.0f),
                   pitch(0.0f),
                   lastX(WIN_WIDTH / 2.0f),
                   lastY(WIN_HEIGHT / 2.0f),
                   firstMouse(true),
                   deltaTime(0.0f),
                   lastFrame(0.0f),
                   currentFrame(0),
                   view(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)) {}

void Camera::processKeyboardInput(GLFWwindow *window)
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 5.0f * deltaTime; // Adjust speed based on time

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            cameraPos += 2.0f * cameraSpeed * cameraFront;
        }
        else
        {
            cameraPos += cameraSpeed * cameraFront;
        }
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
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera->firstMouse)
    {
        camera->lastX = xpos;
        camera->lastY = ypos;
        camera->firstMouse = false;
    }

    float xoffset = xpos - camera->lastX;
    float yoffset = camera->lastY - ypos; // Reversed since y-coordinates go from bottom to top
    camera->lastX = xpos;
    camera->lastY = ypos;

    float sensitivity = 0.1f; // Adjust sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    // Constrain pitch to avoid flipping
    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;

    // Update cameraFront based on yaw and pitch
    glm::vec3 front;
    front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    front.y = sin(glm::radians(camera->pitch));
    front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->cameraFront = glm::normalize(front);
}