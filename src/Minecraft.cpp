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

const unsigned int Block::indices[] = {
    0, 1, 2, 2, 3, 0,       // Back face
    4, 5, 6, 6, 7, 4,       // Front face
    8, 9, 10, 10, 11, 8,    // Left face
    12, 13, 14, 14, 15, 12, // Right face
    16, 17, 18, 18, 19, 16, // Bottom face
    20, 21, 22, 22, 23, 20  // Top face
};
std::unordered_map<std::string, uint8_t> Block::ID_Block_Map = {{"air", 0}, 
                                                                {"grass block", 1}, 
                                                                {"stone", 2}, 
                                                                {"dirt", 3}};
Block::Block(std::string type)
{
    if (ID_Block_Map.find(type) == ID_Block_Map.end())
    {
        std::cerr << "Invalid Block type!\n";
        return;
    }
    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    va.addBuffer(vb, layout);
    IndexBuffer ib(indices, 36);

    Texture atlas("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/texture-atlas.png");
    atlas.bind();
    // code to load the texture according to the type
    ID = ID_Block_Map[type];
    switch (ID)
    {
    case 0: // air
        break;
    case 1: // grass_block
        break;
    case 2: // stone
        break;
    case 3: // dirt
        break;
    }
}

void chunk::drawFlat()
{
}

