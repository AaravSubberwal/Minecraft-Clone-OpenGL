#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <vector>

#include "textures.h"
#include "Renderer.h"
#include "shader.h"
#include "Camera.h"

class World
{
private:
    Shader &shader;
    Texture atlas;

    chunk ourChunk;

public:
    World(Shader &shader);
    ~World();
};

class chunk
{
private:
    float xPos, yPos, zPos;
    uint8_t blockdata[16][128][16]; // 32KB lfg

public:
    chunk(); // takes in what kind of a vhunk it is
    ~chunk();
    void setFlat();
    void draw();
    void generateMesh();
};

class Block
{
private:
    static const std::unordered_map<std::string, uint8_t> NameIDRegistry;
    static const std::unordered_map<uint8_t, std::vector<std::pair<int, int>>> IDTexRegistry;

    std::string type;
    uint8_t ID;
    std::vector<face> faces; // back, front, left, right, bottom, top

public:
    static float vertices[];
    static unsigned int indices[];
    Block(const std::string &type);
    ~Block() = default;

    void terrimummy(float (&arr)[120]);
};

class face
{
private:
    static constexpr float atlasSize = 16.0f;           // 16x16 grid (each cell is 16x16 pixels)
    static constexpr float cellSize = 1.0f / atlasSize; // Each texture takes 1/16th of the atlas (0.0625)

    std::vector<float> texCoordsArray; // Stores texture coordinates for the face

public:
    face(int row, int col);
    const std::vector<float> &getTexCoords() const { return texCoordsArray; }
};
// ig i would have make an instance of each block about to be used before i can use them
// just having the proper offsets ready before hand might be better but this code works sooo fuck off