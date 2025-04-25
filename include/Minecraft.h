#pragma once
// https://0fps.net/2012/01/14/an-analysis-of-minecraft-like-engines/
#include <unordered_map>
#include <array>
#include <vector>
#include <memory>
#include <iostream>
#include <random>

#include "textures.h"
#include "Renderer.h"
#include "shader.h"
#include "Camera.h"
#include "Window.h"
#include "FastNoiseLite.h"
#include "UI.h"

#define RIGHT 0
#define LEFT 1
#define TOP 2
#define BOTTOM 3
#define FRONT 4
#define BACK 5

class World;
class Chunk;

class Chunk
{
private:
    glm::ivec2 position;

    GLuint vao, vbo, ebo;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    bool isFaceVisible(uint8_t i, uint8_t j, uint8_t k, int face) const;
    void uploadBuffers();

public:
    uint8_t blockdata[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]; // ~32KB per chunk

    Chunk(const glm::ivec2 &position);
    ~Chunk();

    static World *world;
    static FastNoiseLite *noise;

    bool withinRenderDistance;
    bool beenModified;

    Chunk(const Chunk &) = delete; // disable copying
    Chunk &operator=(const Chunk &) = delete;

    inline const glm::ivec2 getPosition() { return position; }
    void buildMesh();
    void genTerrain();
    void render();
};

class World
{
private:
    Window window;
    Shader shader3D;
    Texture atlas;
    Camera camera;
    UI ui;
    FastNoiseLite noise;

    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>> world_Map;
    std::vector<Chunk *> activeChunks;
    uint8_t renderDistance;
    unsigned int frames;

    Chunk *addChunk(const glm::ivec2 &position);
    void removeChunk(const glm::ivec2 &position);
    bool hasChunk(const glm::ivec2 &position) const;

    void updateChunks(glm::ivec2 offset);
    void renderChunks();

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
