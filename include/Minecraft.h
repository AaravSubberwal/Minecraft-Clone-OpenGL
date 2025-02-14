#pragma once

#include <unordered_map>
#include <array>
#include <vector>
#include <cstdint>
#include <cstring>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "textures.h"
#include "Renderer.h"
#include "shader.h"
#include "Camera.h"

#define CHUNK_HEIGHT 128
#define CHUNK_SIZE 16

struct Vertex {
    glm::vec3 position;
    uint8_t texIndex;
};

class chunk {
private:
    float xPos, yPos, zPos;
    uint8_t blockdata[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]; // ~32KB per chunk
    GLuint vao, vbo, ebo;
    
    size_t indexCount; // Number of indices in our mesh

    bool isBlockVisible(uint8_t x, uint8_t y, uint8_t z);
    bool isFaceVisible(uint8_t x, uint8_t y, uint8_t z, int face);
    
    public:
    void generateMesh(); // Builds the VAO/VBO/EBO
    chunk(float x, float y, float z); // Constructor: position in world space.
    ~chunk();
    void setFlat();
    void render();
    void draw(); // For additional draw logic, if needed.
};
class World {
private:
    Shader &shader;
    Texture atlas;
    chunk mychunk;
public:
    World(Shader &shader);
    ~World();
    void render();
};


// External declarations for texture mapping and face vertices.
extern const std::unordered_map<uint8_t, std::array<uint8_t, 6>> blockTextures;
uint8_t blockTextureLookup(uint8_t blockID, int face);

extern const glm::vec3 faceVertices[6][4];
