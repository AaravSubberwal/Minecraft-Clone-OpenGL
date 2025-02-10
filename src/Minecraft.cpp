#include "Minecraft.h"

const unsigned int Block::indices[36] = {
    0, 1, 2, 2, 3, 0,       // Back face
    4, 5, 6, 6, 7, 4,       // Front face
    8, 9, 10, 10, 11, 8,    // Left face
    12, 13, 14, 14, 15, 12, // Right face
    16, 17, 18, 18, 19, 16, // Top face
    20, 21, 22, 22, 23, 20  // Bottom face
};

float Block::vertices[96] = {
    // Back face
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,

    // Front face
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    // Left face
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    // Right face
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,

    // Top face
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    // Bottom face
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f};

Block::Block(std::string type)
{
    if (ID_Block_Map.find(type) == ID_Block_Map.end())
    {
        std::cerr << "Invalid Block type!\n";
        return;
    }
    ID= ID_Block_Map[type];
    switch (ID)
    {
    case 0: // grass_block
        break;
    case 1: // dirt
        break;
    case 2: // stone
        break;
    }
}
