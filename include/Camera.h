#pragma once

#include "shader.h"
#include "Window.h"

#include <array>

#define CHUNK_HEIGHT 128
#define CHUNK_SIZE 16

class Camera
{
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

    uint8_t render_Distance;
    glm::mat4 projection;
    glm::ivec2 currentChunk;
    glm::ivec2 newChunkOffset;
    std::array<glm::vec4, 6> frustumPlanes;

    void updateFrustumPlanes();

public:
    Window &window;
    void processKeyboardInput(GLFWwindow *window);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    glm::mat4 view;

    Camera(Window &window, uint8_t render_Distance);
    ~Camera() = default;

    inline void teleport(float x, float y, float z) { cameraPos = glm::vec3(x, y, z); }
    inline glm::vec3 getPlayerPos() { return cameraPos; }
    inline uint8_t getRenderDistance() { return render_Distance; }
    inline glm::vec2 getPlayerChunk() { return currentChunk; }
    inline void recieveProjection(glm::mat4 proj) { projection = proj; }
    inline glm::ivec2 getNewChunkOffset() { return newChunkOffset; }
    
    bool isChunkInFrustum(const glm::ivec2 &position);
    bool didPlayerChunkChange;
};
