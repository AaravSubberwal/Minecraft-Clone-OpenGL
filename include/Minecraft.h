#pragma once

#include <unordered_map>
#include <array>
#include <vector>

#include "textures.h"
#include "Renderer.h"
#include "shader.h"
#include "Camera.h"

#define CHUNK_HEIGHT 128
#define CHUNK_SIZE 16
#define WIN_WIDTH 2560
#define WIN_HEIGTH 1600

struct Vertex
{
    glm::vec3 position;
    uint8_t texIndex;
};

class Chunk
{
private:
    float chunkX, chunkY, chunkZ; // Chunk corner coordinate. Figure out which one!!
    uint8_t blockdata[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]; // ~32KB per chunk

    GLuint vao, vbo, ebo;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    bool isFaceVisible(uint8_t x, uint8_t y, uint8_t z, int face) const;
    void uploadBuffers();

public:
    Chunk(float x, float y, float z); // Constructor: position in world space.
    ~Chunk();

    void buildMesh();
    void setFlat();
    void render();
};

class World
{
private:
    Shader &shader;
    Camera &camera;
    Texture atlas;
    Chunk chunk;

public:
    World(Shader &shader, Camera &camera);
    ~World() = default;
    void render();
};

// External declarations for texture mapping and face vertices.
extern const std::unordered_map<uint8_t, std::array<uint8_t, 6>> blockTextures;
uint8_t faceTexIndexLookup(uint8_t blockID, int face);

extern const glm::vec3 faceVertices[6][4];
