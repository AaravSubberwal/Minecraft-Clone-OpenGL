#include "Minecraft.h"

float Block::vertices[] = {
    // Positions          // Texture Coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Back face
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Front face
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Left face
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Right face
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom face
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Top face
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f};

const std::unordered_map<std::string, uint8_t> Block::NameIDRegistry = {
    {"air",0},
    {"grass_block", 1},
    {"dirt", 2},
    {"stone", 3}};

const std::unordered_map<uint8_t, std::vector<std::pair<int, int>>> Block::IDTexRegistry = {
    {1, {
            {3, 15}, // Back
            {3, 15}, // Front
            {3, 15}, // Left
            {3, 15}, // Right
            {2, 15}, // Bottom (Dirt)
            {1, 0}  // Top (Grass)
        }},
    {2, {{2, 15}, {2, 15}, {2, 0}, {2, 15}, {2, 15}, {2, 15}}},
    {3, {{1, 15}, {1, 15}, {1, 15}, {1, 15}, {1, 15}, {1, 15}}}};

Block::Block(const std::string &type) : type(type)
{
    auto it = NameIDRegistry.find(type);
    if (it != NameIDRegistry.end())
    {
        ID = it->second;
    }
    else
    {
        throw std::runtime_error("Unknown block type: " + type);
    }

    // Initialize faces based on texture mapping
    auto texIt = IDTexRegistry.find(ID);
    if (texIt != IDTexRegistry.end())
    {
        const auto &texCoords = texIt->second;
        for (const auto &coord : texCoords)
        {
            faces.emplace_back(coord.first, coord.second);
        }
    }
    else
    {
        throw std::runtime_error("Texture mapping not found for block ID: " + std::to_string(ID));
    }
}

face::face(int row, int col)
    : texCoordsArray{
          col * cellSize, (row + 1) * cellSize,      
          (col + 1) * cellSize, (row + 1) * cellSize, 
          (col + 1) * cellSize, row * cellSize,       
          col * cellSize, row * cellSize              
      }
{
}
// uint8_t blockdata[16][128][16];

void chunk::setFlat()
{
    memset(blockdata, 0, sizeof(blockdata)); // Set everything to 0

    for (uint8_t x = 0; x < 16; x++)
    {
        for (uint8_t z = 0; z < 16; z++)
        {
            blockdata[x][0][z] = 1; // Set the bottom layer to 1
        }
    }
}

void chunk::draw()
{ // send the vertices for a single cube to the gpu. every other cube must be drawn from translating the personal cube coords to
  // local space. however only the cube faces visible to the camera must be drawn to the screen
}

void chunk::generateMesh()
{ // figure out what bkocks need to be drawn. some sort of algo to generate all the vertices
    for (uint8_t x = 0; x < 16; x++)
    {
        for (uint8_t y = 0; y < 128; y++)
        {
            for (uint8_t z = 0; z < 16; z++)
            {
                if (blockdata[x][y][z] != 0)
                { // reuse the position vertices and add specific texture offset for each block
                }
            }
        }
    }
}
