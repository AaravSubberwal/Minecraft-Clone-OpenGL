#pragma once

#include <unordered_map>
#include <array>
#include <vector>
#include <memory>
#include <iostream>

#include "textures.h"
#include "Renderer.h"
#include "shader.h"
#include "Camera.h"
#include "Window.h"

#define CHUNK_HEIGHT 128
#define CHUNK_SIZE 16

#define RIGHT 0
#define LEFT 1
#define TOP 2
#define BOTTOM 3
#define FRONT 4
#define BACK 5

struct Vertex
{
    glm::ivec3 position;
    uint8_t texIndex;
};



// Add hash function by specializing std::hash
namespace std {
    template<>
    struct hash<chunkPos> {
        size_t operator()(const chunkPos& pos) const {
            // Combine the hashes of x, y, and z
            size_t h1 = std::hash<int>{}(pos.x);
            size_t h3 = std::hash<int>{}(pos.z);
            
            // You can use any hash combining function. Here's a simple one:
            return h1  ^ (h3 << 2);
        }
    };
}

class Chunk
{
private:
    int chunkX, chunkZ;                              // Chunk corner coordinate. Figure out which one!!
    uint8_t blockdata[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]; // ~32KB per chunk

    GLuint vao, vbo, ebo;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    bool isFaceVisible(uint8_t i, uint8_t j, uint8_t k, int face) const;
    void uploadBuffers();

public:
    Chunk(int x, int z); // Constructor: position in world space.
    ~Chunk();

    Chunk(const Chunk &) = delete; //disable copying
    Chunk &operator=(const Chunk &) = delete;

    void buildMesh();
    void setFlat();
    void render();
};

class World
{
private:
    Window window;
    Shader shader;
    Texture atlas;
    Camera camera;

    std::unordered_map<chunkPos, std::unique_ptr<Chunk>> world_Map;
    glm::ivec2 lastPlayerChunk = glm::ivec2(-9999);
    uint8_t renderDistance;

    void addChunk(const glm::ivec2& position);
    void removeChunk(const glm::ivec2& position);
    Chunk* getChunk(const glm::ivec2& position)const;
    bool hasChunk(const glm::ivec2& position) const;

    void draw();
    
public:
    World();
    ~World() = default;
    void render();
    int shouldClose();
};

extern const std::unordered_map<uint8_t, std::array<uint8_t, 6>> blockTextures;
uint8_t faceTexIndexLookup(uint8_t blockID, int face);
extern const glm::ivec3 faceVertices[6][4];

