#include "Minecraft.h"

World::World(GLFWwindow *window) : shader("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/vertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/fragmentShader.glsl"), window(window),
                                   atlas("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/terrain.png"), chunk(0.0f, 0.0f, 0.0f)
{
    glfwSetCursorPosCallback(window, Camera::mouse_callback);
    glfwSetWindowUserPointer(window, &camera);

    enableDebugging();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//===========================================================================================================================/
 
    atlas.bind();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGTH, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);

    shader.setUniformMatrix4fv("u_model", model);
    shader.setUniformMatrix4fv("u_projection", projection);
    shader.setUniform1i("u_atlas", 0);
    shader.setUniform3f("u_grassTint", 0.5f, 0.8f, 0.4f);

    chunk.setFlat();
    chunk.buildMesh();
}

void World::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.processKeyboardInput(window);
    shader.setUniformMatrix4fv("u_view", camera.view);

    chunk.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

Chunk::Chunk(float x, float y, float z) : chunkX(x), chunkY(y), chunkZ(z)
{
    memset(blockdata, 0, sizeof(blockdata));
}

Chunk::~Chunk() // NO IDEA WHAT WILL HAPPEN IF buildMesh() hasnt been called before
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Chunk::buildMesh() // called when chunk loads for the first time and when reloaded
{
    vertices.reserve(100);
    unsigned int indexOffset = 0;
    for (uint8_t i = 0; i < CHUNK_SIZE; ++i)
    {
        for (uint8_t j = 0; j < CHUNK_HEIGHT; ++j)
        {
            for (uint8_t k = 0; k < CHUNK_SIZE; ++k)
            {
                uint8_t blockID = blockdata[i][j][k];
                if (blockID == 0)
                    continue; // Skip air blocks.
                // For each face, if visible, add its vertices and indices.
                for (int face = 0; face < 6; face++)
                {
                    if (!isFaceVisible(i, j, k, face))
                        continue;

                    // For each face, add 4 vertices.
                    for (int v = 0; v < 4; v++)
                    {
                        Vertex vertex;
                        vertex.position = glm::vec3(chunkX + i, chunkY + j, chunkZ + k) + faceVertices[face][v];
                        vertex.texIndex = faceTexIndexLookup(blockID, face);
                        vertices.push_back(vertex);
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
    uploadBuffers();
}

void Chunk::uploadBuffers()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void *)offsetof(Vertex, texIndex));
    glEnableVertexAttribArray(1);
}

bool Chunk::isFaceVisible(uint8_t x, uint8_t y, uint8_t z, int face) const
{
    // For now, always return true. You can add neighbor checks later.
    return true;
}

void Chunk::render()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Chunk::setFlat()
{
    for (uint8_t x = 0; x < CHUNK_SIZE; x++)
    {
        for (uint8_t z = 0; z < CHUNK_SIZE; z++)
        {
            blockdata[x][0][z] = 1;
        }
    }
}

// a Hashmap that maps BlockID to an array of texture indexes which correspond to each face of the block
const std::unordered_map<uint8_t, std::array<uint8_t, 6>> blockTextures = {
    {1, {3, 3, 0, 2, 3, 3}},  // grass_block
    {2, {1, 1, 1, 1, 1, 1}},  // stone
    {3, {2, 2, 2, 2, 2, 2}},  // dirt
    {4, {4, 4, 4, 4, 4, 4}}}; // oak planks
// right, left, top, bottom, front, back

// Lookup function to get the texture atlas index for a given block and face.
uint8_t faceTexIndexLookup(uint8_t blockID, int face)
{
    auto it = blockTextures.find(blockID);
    if (it != blockTextures.end())
    {
        return it->second[face];
    }
    return 26; // wierd purple block
}
const glm::vec3 faceVertices[6][4] = {
    // Right (+X)
    {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
    // Left (-X)
    {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
    // Top (+Y)
    {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    // Bottom (-Y)
    {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    // Front (+Z)
    {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    // Back (-Z)
    {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}
};

