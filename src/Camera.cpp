#include "Camera.h"

Camera::Camera(Window &window, uint8_t render_Distance) : cameraPos(glm::vec3(0.0f, 5.0f, 0.0f)),
                                                          cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), render_Distance(render_Distance),
                                                          cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), window(window),
                                                          yaw(-90.0f),
                                                          pitch(0.0f),
                                                          lastX(window.getWidth() / 2.0f),
                                                          lastY(window.getHeight() / 2.0f),
                                                          firstMouse(true),
                                                          deltaTime(0.0f),
                                                          lastFrame(0.0f),
                                                          currentFrame(0), currentChunk(glm::ivec2(glm::floor(cameraPos.x / 16), glm::floor(cameraPos.z / 16))), view(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)) {}

bool Camera::isChunkInFrustum(const glm::ivec2 &position)
{
    // Get chunk AABB using defined constants
    glm::vec3 minCorner(position.x * CHUNK_SIZE, 0, position.y * CHUNK_SIZE);
    glm::vec3 maxCorner(minCorner.x + CHUNK_SIZE, CHUNK_HEIGHT, minCorner.z + CHUNK_SIZE);

    // Check if AABB is inside the frustum
    for (const auto &plane : frustumPlanes)
    {
        glm::vec3 normal(plane.x, plane.y, plane.z);
        float d = plane.w;

        // Find the *most outside* corner in the plane’s normal direction
        glm::vec3 positiveCorner = {
            normal.x > 0 ? maxCorner.x : minCorner.x,
            normal.y > 0 ? maxCorner.y : minCorner.y,
            normal.z > 0 ? maxCorner.z : minCorner.z};

        // If this corner is outside, discard the chunk
        if (glm::dot(normal, positiveCorner) + d < 0)
            return false;
    }

    return true; // Chunk is inside the frustum
}

void Camera::updateFrustumPlanes()
{
    glm::mat4 vp = projection * view;
    frustumPlanes[0] = glm::vec4(vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0], vp[3][3] + vp[3][0]); // Left
    frustumPlanes[1] = glm::vec4(vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0], vp[3][3] - vp[3][0]); // Right
    frustumPlanes[2] = glm::vec4(vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1], vp[3][3] + vp[3][1]); // Bottom
    frustumPlanes[3] = glm::vec4(vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1], vp[3][3] - vp[3][1]); // Top
    frustumPlanes[4] = glm::vec4(vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2], vp[3][3] + vp[3][2]); // Near
    frustumPlanes[5] = glm::vec4(vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2], vp[3][3] - vp[3][2]); // Far

    // Normalize planes
    for (auto &plane : frustumPlanes)
        plane /= glm::length(glm::vec3(plane)); // Normalize the normal (A, B, C)
}

void Camera::processKeyboardInput(GLFWwindow *window)
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    currentChunk = glm::ivec2(glm::floor(cameraPos.x / 16), glm::floor(cameraPos.z / 16));
    didPlayerChunkChange = true;

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
    
    if (glm::ivec2(glm::floor(cameraPos.x / 16), glm::floor(cameraPos.z / 16)) != currentChunk)
    {
        didPlayerChunkChange = true;
    }
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    updateFrustumPlanes();
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
    camera->updateFrustumPlanes();
}