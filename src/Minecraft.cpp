#include "Minecraft.h"

World::World(Shader &shader) : shader(shader),
                               atlas("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/terrain.png"), mychunk(0.0f, 0.0f, 0.0f)
{
    atlas.bind();
    shader.setUniform1i("u_atlas", 0);
    shader.setUniform3f("u_grassTint", 0.5f, 0.8f, 0.4f);

    mychunk.setFlat(); // e.g., generate a flat terrain layout
    mychunk.generateMesh();
}

World::~World()
{
}

void World::render()
{
    mychunk.render();
}

chunk::chunk(float x, float y, float z) : xPos(x), yPos(y), zPos(z), indexCount(0)
{
    // Initialize block data to 0.
    memset(blockdata, 0, sizeof(blockdata));
}

void chunk::generateMesh()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int indexOffset = 0;
    // Iterate through all blocks in the chunk.
    for (uint8_t x = 0; x < CHUNK_SIZE; ++x)
    {
        for (uint8_t y = 0; y < CHUNK_HEIGHT; ++y)
        {
            for (uint8_t z = 0; z < CHUNK_SIZE; ++z)
            {
                uint8_t blockID = blockdata[x][y][z];
                if (blockID == 0)
                    continue; // Skip air blocks.

                // For each face, if visible, add its vertices and indices.
                for (int face = 0; face < 6; face++)
                {
                    if (!isFaceVisible(x, y, z, face))
                        continue;

                    // For each face, add 4 vertices.
                    for (int v = 0; v < 4; v++)
                    {
                        Vertex vert;
                        vert.position = glm::vec3(xPos + x, yPos + y, zPos + z) + faceVertices[face][v];
                        vert.texIndex = blockTextureLookup(blockID, face);
                        vertices.push_back(vert);
                    }
                    // Add 6 indices to form two triangles for the face.
                    indices.push_back(indexOffset + 0);
                    indices.push_back(indexOffset + 1);
                    indices.push_back(indexOffset + 2);
                    indices.push_back(indexOffset + 2);
                    indices.push_back(indexOffset + 3);
                    indices.push_back(indexOffset + 0);
                    indexOffset += 4;
                }
            }
        }
    }
    indexCount = indices.size();

    // Generate and bind buffers.
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    // Upload vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Upload index data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // Set vertex attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void *)offsetof(Vertex, texIndex));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

chunk::~chunk()
{
    // Clean up GPU buffers.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

bool chunk::isBlockVisible(uint8_t x, uint8_t y, uint8_t z)
{
    // Here we simply consider a block "visible" if it is non-air.
    return blockdata[x][y][z] != 0;
}

bool chunk::isFaceVisible(uint8_t x, uint8_t y, uint8_t z, int face)
{
    // For now, always return true. You can add neighbor checks later.
    return true;
}

void chunk::setFlat()
{
    // Clear block data.
    memset(blockdata, 0, sizeof(blockdata));
    // Set the bottom layer to block ID 1 (for example, Grass).
    for (uint8_t x = 0; x < CHUNK_SIZE; x++)
    {
        for (uint8_t z = 0; z < CHUNK_SIZE; z++)
        {
            blockdata[x][0][z] = 3;
        }
    }
}

void chunk::render()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void chunk::draw()
{
    // Currently, simply call render.
    render();
}
// Block ID 1 (Grass): { top, bottom, right, left, front, back }
// Block ID 2 (Stone): Same texture on all faces.
// Block ID 3 (Dirt): Same texture on all faces.
const std::unordered_map<uint8_t, std::array<uint8_t, 6>> blockTextures = {
    {1, {3, 3, 0, 2, 3, 3}},
    {2, {1, 1, 1, 1, 1, 1}},
    {3, {2, 2, 2, 2, 2, 2}}};

// Lookup function to get the texture atlas index for a given block and face.
uint8_t blockTextureLookup(uint8_t blockID, int face)
{
    auto it = blockTextures.find(blockID);
    if (it != blockTextures.end())
    {
        return it->second[face];
    }
    return 0; // Default to texture 0 if blockID not found.
}
const glm::vec3 faceVertices[6][4] = {
    // Right (+X)
    {{1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}},
    // Left (-X)
    {{0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {0, 0, 0}},
    // Top (+Y)
    {{0, 1, 0}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}},
    // Bottom (-Y)
    {{0, 0, 1}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}},
    // Front (+Z)
    {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}},
    // Back (-Z)
    {{1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0}}};
