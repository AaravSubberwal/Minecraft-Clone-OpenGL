#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <array>
#include <vector>

#include "textures.h"
#include "Renderer.h"
#include "shader.h"

class face
{
private:
    static constexpr float atlasSize = 16.0f;           // 16x16 grid (each cell is 16x16 pixels)
    static constexpr float cellSize = 1.0f / atlasSize; // Each texture takes 1/16th of the atlas (0.0625)

    std::vector<float> texCoordsArray; // Stores texture coordinates for the face

public:
    face(int col, int row)
    {
        float u = col * cellSize;
        float v = row * cellSize;
        texCoordsArray = {
            u, v + cellSize,            // Top-left
            u + cellSize, v + cellSize, // Top-right
            u + cellSize, v,            // Bottom-right
            u, v                        // Bottom-left
        };
    }

    const std::vector<float> &getTexCoords() const { return texCoordsArray; }
};

class Block
{
private:
    static float vertices[];
    static unsigned int indices[];
    static const std::unordered_map<std::string, uint8_t> NameIDRegistry;
    static const std::unordered_map<uint8_t, std::vector<std::pair<int, int>>> IDTexRegistry;

    std::string type;
    uint8_t ID;
    std::vector<face> faces; // back, front, left, right, bottom, top

public:
    Block(const std::string &type);
    ~Block() = default;
};



class chunk
{
private:
    float xPos, yPos, zPos;
    uint8_t blockdata[16][128][16]; // 32KB lfg

public:
    void setFlat();
    void draw();
    void generateMesh();
};
