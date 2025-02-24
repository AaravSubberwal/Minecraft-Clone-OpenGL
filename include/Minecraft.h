#pragma once

#include <unordered_map>
#include <array>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_set>

#include "textures.h"
#include "Renderer.h"
#include "shader.h"
#include "Camera.h"
#include "Window.h"

#define RIGHT 0
#define LEFT 1
#define TOP 2
#define BOTTOM 3
#define FRONT 4
#define BACK 5

class World;

class Chunk
{
private:
    glm::ivec2 position;

    GLuint vao, vbo, ebo;
    World *world;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    bool isFaceVisible(uint8_t i, uint8_t j, uint8_t k, int face) const;
    void uploadBuffers();

public:
    uint8_t blockdata[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]; // ~32KB per chunk

    Chunk(const glm::ivec2 &position, World *world); // Constructor: position in world space.
    ~Chunk();

    bool withinRenderDistance;
    bool beenModified;
    Chunk(const Chunk &) = delete; // disable copying
    Chunk &operator=(const Chunk &) = delete;

    inline const glm::ivec2 getPosition() { return position; }
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

    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>> world_Map;
    std::vector<Chunk *> activeChunks;
    uint8_t renderDistance;
    unsigned int frames;

    Chunk *addChunk(const glm::ivec2 &position);
    void removeChunk(const glm::ivec2 &position);
    bool hasChunk(const glm::ivec2 &position) const;

    void draw();

public:
    Chunk *getChunk(const glm::ivec2 &position) const;
    World();
    ~World() = default;
    void render();
    int shouldClose();
};

extern const std::unordered_map<uint8_t, std::array<uint8_t, 6>> blockTextures;
uint8_t faceTexIndexLookup(uint8_t blockID, int face);
extern const glm::ivec3 faceVertices[6][4];
