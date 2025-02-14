#include "Minecraft.h"
World::World(Shader &shader):shader(shader), atlas("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/terrain.png")
{
    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.push<float>(3);
    va.addBuffer(vb, layout);
    IndexBuffer ib(Block::indices, 36);
// we now have vertex buffer that stores the local positions for a block in vram

    atlas.bind();
    shader.setUniform1i("u_atlas",0);
}

// uint8_t blockdata[16][128][16];
chunk::chunk()
{
}

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
                    glm::vec3 cubeposition((float)x, (float)y, (float)z);
                }
            }
        }
    }
}

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
float Block::vertices[] = {
    // Positions          
    -0.5f, -0.5f, -0.5f, // Back face
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f, // Front face
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f, // Left face
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,

    0.5f, -0.5f, -0.5f, // Right face
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f, // Bottom face
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, 0.5f, -0.5f, // Top face
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f
};


const float atlasSize = 16.0f;           // 16x16 grid (each cell is 16x16 pixels)
const float cellSize = 1.0f / atlasSize; // Each texture takes 1/16th of the atlas (0.0625)

const int col = 3, row = 15 - (14); // Change these to select different textures
const float u = col * cellSize;
const float v = row * cellSize;

float vertices[] = {
    // Positions          // Texture Coords (Atlas-based)
    0.0f, 0.0f, 0.0f, u, v + cellSize, // Back face
    1.0f, 0.0f, 0.0f, u + cellSize, v + cellSize,
    1.0f, 1.0f, 0.0f, u + cellSize, v,
    0.0f, 1.0f, 0.0f, u, v,

    0.0f, 0.0f, 1.0f, u, v + cellSize, // Front face
    1.0f, 0.0f, 1.0f, u + cellSize, v + cellSize,
    1.0f, 1.0f, 1.0f, u + cellSize, v,
    0.0f, 1.0f, 1.0f, u, v,

    0.0f, 0.0f, 0.0f, u, v + cellSize, // Left face
    0.0f, 0.0f, 1.0f, u + cellSize, v + cellSize,
    0.0f, 1.0f, 1.0f, u + cellSize, v,
    0.0f, 1.0f, 0.0f, u, v,

    1.0f, 0.0f, 0.0f, u, v + cellSize, // Right face
    1.0f, 0.0f, 1.0f, u + cellSize, v + cellSize,
    1.0f, 1.0f, 1.0f, u + cellSize, v,
    1.0f, 1.0f, 0.0f, u, v,

    0.0f, 0.0f, 0.0f, u, v + cellSize, // Bottom face
    1.0f, 0.0f, 0.0f, u + cellSize, v + cellSize,
    1.0f, 0.0f, 1.0f, u + cellSize, v,
    0.0f, 0.0f, 1.0f, u, v,

    0.0f, 1.0f, 0.0f, u, v + cellSize, // Top face
    1.0f, 1.0f, 0.0f, u + cellSize, v + cellSize,
    1.0f, 1.0f, 1.0f, u + cellSize, v,
    0.0f, 1.0f, 1.0f, u, v};
unsigned int Block::indices[] = {
    0, 1, 2, 2, 3, 0,       // Back face
    4, 5, 6, 6, 7, 4,       // Front face
    8, 9, 10, 10, 11, 8,    // Left face
    12, 13, 14, 14, 15, 12, // Right face
    16, 17, 18, 18, 19, 16, // Bottom face
    20, 21, 22, 22, 23, 20  // Top face
};
const std::unordered_map<std::string, uint8_t> Block::NameIDRegistry = {
    {"air", 0},
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
            {1, 0}   // Top (Grass)
        }},
    {2, {{2, 15}, {2, 15}, {2, 0}, {2, 15}, {2, 15}, {2, 15}}},
    {3, {{1, 15}, {1, 15}, {1, 15}, {1, 15}, {1, 15}, {1, 15}}}};

face::face(int row, int col)
    : texCoordsArray{
          col * cellSize, (row + 1) * cellSize,
          (col + 1) * cellSize, (row + 1) * cellSize,
          (col + 1) * cellSize, row * cellSize,
          col * cellSize, row * cellSize}
{
}
