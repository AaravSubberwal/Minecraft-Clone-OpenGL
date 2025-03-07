#include "Minecraft.h"

World::World()
    : window(), camera(window, 7), shader("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/vertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/fragmentShader.glsl"),
      atlas("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/terrain.png")
{
    glfwSetCursorPosCallback(window.p_GLFWwindow(), Camera::mouse_callback);
    glfwSetWindowUserPointer(window.p_GLFWwindow(), &camera);

    enableDebugging();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //===========================================================================================================================/

    atlas.bind();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window.getWidth() / (float)window.getHeight(), 0.1f, 500.0f);
    camera.recieveProjection(projection);

    glm::mat4 model = glm::mat4(1.0f);

    shader.setUniformMatrix4fv("u_model", model);
    shader.setUniformMatrix4fv("u_projection", projection);
    shader.setUniform1i("u_atlas", 0);
    shader.setUniform3f("u_grassTint", 0.5f, 0.8f, 0.4f);

    renderDistance = camera.getRenderDistance();
    frames = 0;
    activeChunks.reserve((2 * renderDistance + 1) * (2 * renderDistance + 1));
    updateChunks(camera.getPlayerChunk());
}

void World::render()
{
    glClearColor(127.0f / 255.0f, 178.0f / 255.0f, 255.0f / 255.0f, 1.0f); // skyyy
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera.processKeyboardInput(window.p_GLFWwindow());
    shader.setUniformMatrix4fv("u_view", camera.view);
    
    glm::ivec2 currentPlayerChunk = camera.getPlayerChunk();
    glm::ivec2 newChunkOffset = camera.getNewChunkOffset();
    
    if (newChunkOffset != glm::ivec2(0))
        updateChunks(currentPlayerChunk);
    renderChunks();

    glfwSwapBuffers(window.p_GLFWwindow());
    glfwPollEvents();
}

void World::updateChunks(glm::ivec2 currentPlayerChunk)
{ // using newChunkOffset calculate which chunks need to be added and removed
    for (int x = currentPlayerChunk.x - renderDistance; x <= currentPlayerChunk.x + renderDistance; x++)
    {
        for (int y = currentPlayerChunk.y - renderDistance; y <= currentPlayerChunk.y + renderDistance; y++)
        {
            glm::ivec2 position = glm::ivec2(x, y);
            if (!hasChunk(position)) // Only add new chunks
            {
                Chunk *chunk = addChunk(position);
                chunk->withinRenderDistance = true;
                chunk->setFlat();
                chunk->buildMesh();
            }
        }
    }
}

void World::renderChunks()
{
    glm::ivec2 currentPlayerChunk = camera.getPlayerChunk();
    frames++;
    for (auto &chunk : activeChunks)
    {
        glm::vec2 position = chunk->getPosition();
        if (position.x <= currentPlayerChunk.x + renderDistance && position.x >= currentPlayerChunk.x - renderDistance && position.y <= currentPlayerChunk.y + renderDistance && position.y >= currentPlayerChunk.y - renderDistance)
        {
            if (camera.isChunkInFrustum(position)) 
            {
                chunk->render();
            }
        }
    }
}

Chunk *World::addChunk(const glm::ivec2 &position)
{
    auto chunk = std::make_unique<Chunk>(position, this);
    Chunk *chunkPtr = chunk.get();
    world_Map.try_emplace(position, std::move(chunk));
    activeChunks.emplace_back(chunkPtr);
    return chunkPtr;
}

void World::removeChunk(const glm::ivec2 &position)
{
    auto it = world_Map.find(position);
    if (it != world_Map.end())
    {
        Chunk *chunkPtr = it->second.get();
        for (size_t i = 0; i < activeChunks.size(); i++)
        {
            if (activeChunks[i] == chunkPtr)
            {
                activeChunks[i] = activeChunks.back();
                activeChunks.pop_back();
                break;
            }
        }
        world_Map.erase(it);
    }
}

Chunk *World::getChunk(const glm::ivec2 &position) const
{
    auto it = world_Map.find(position);
    if (it != world_Map.end())
    {
        return it->second.get(); // Return raw pointer to the Chunk
    }
    return nullptr;
}

bool World::hasChunk(const glm::ivec2 &position) const
{
    return world_Map.find(position) != world_Map.end();
}

int World::shouldClose()
{
    return glfwWindowShouldClose(window.p_GLFWwindow());
}

//======================================================================================================================

Chunk::Chunk(const glm::ivec2 &position, World *world) : position(position), world(world)
{
    beenModified = false;
    withinRenderDistance = true;
    memset(blockdata, 0, sizeof(blockdata));
}

Chunk::~Chunk() // NO IDEA WHAT WILL HAPPEN IF buildMesh() hasnt been called before
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

//stupid Algoooo greedy meshing pls
void Chunk::buildMesh() // called when chunk loads for the first time and when reloaded
{
    vertices.reserve(100);
    unsigned int indexOffset = 0;
    for (int i = 0; i < CHUNK_SIZE; ++i)
    {
        for (int j = 0; j < CHUNK_HEIGHT; ++j)
        {
            for (int k = 0; k < CHUNK_SIZE; ++k)
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
                        vertex.position = glm::ivec3((position.x * 16) + i, j, (position.y * 16) + k) + faceVertices[face][v];
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

bool Chunk::isFaceVisible(uint8_t i, uint8_t j, uint8_t k, int face) const
{
    static constexpr int offsets[6][3] = {
        {1, 0, 0},  // RIGHT
        {-1, 0, 0}, // LEFT
        {0, 1, 0},  // TOP
        {0, -1, 0}, // BOTTOM
        {0, 0, 1},  // FRONT
        {0, 0, -1}  // BACK
    };

    int ni = i + offsets[face][0];
    int nj = j + offsets[face][1];
    int nk = k + offsets[face][2];

    // If within the current chunk, check normally
    if (ni >= 0 && ni < CHUNK_SIZE &&
        nj >= 0 && nj < CHUNK_HEIGHT &&
        nk >= 0 && nk < CHUNK_SIZE)
    {
        return blockdata[ni][nj][nk] == 0;
    }

    // Neighboring chunk check
    glm::ivec2 neighborPos = position; // Chunk position (not world coords)
    int neighbor_i = ni;
    int neighbor_k = nk;

    if (ni < 0)
    {
        neighborPos.x--;
        neighbor_i = CHUNK_SIZE - 1;
    }
    if (ni >= CHUNK_SIZE)
    {
        neighborPos.x++;
        neighbor_i = 0;
    }
    if (nk < 0)
    {
        neighborPos.y--;
        neighbor_k = CHUNK_SIZE - 1;
    }
    if (nk >= CHUNK_SIZE)
    {
        neighborPos.y++;
        neighbor_k = 0;
    }

    Chunk *neighborChunk = world->getChunk(neighborPos);
    if (!neighborChunk)
        return true; // Assume air if neighbor chunk doesn't exist

    return neighborChunk->blockdata[neighbor_i][nj][neighbor_k] == 0;
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

    glVertexAttribIPointer(0, 3, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void *)offsetof(Vertex, texIndex));
    glEnableVertexAttribArray(1);
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
            blockdata[x][0][z] = 5;
            blockdata[x][1][z] = 3;
            blockdata[x][2][z] = 3;
            blockdata[x][3][z] = 1;
        }
    }
}

// a Hashmap that maps BlockID to an array of texture indexes which correspond to each face of the block
const std::unordered_map<uint8_t, std::array<uint8_t, 6>> blockTextures = {
    {1, {3, 3, 0, 2, 3, 3}},      // grass_block
    {2, {1, 1, 1, 1, 1, 1}},      // stone
    {3, {2, 2, 2, 2, 2, 2}},      // dirt
    {4, {4, 4, 4, 4, 4, 4}},      // oak planks
    {5, {17, 17, 17, 17, 17, 17}} // bedrock
};
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

const glm::ivec3 faceVertices[6][4] = {
    // Right (+X)
    {{1, 0, 1}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1}},
    // Left (-X)
    {{0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}},
    // Top (+Y)
    {{0, 1, 0}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}},
    // Bottom (-Y)
    {{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}},
    // Front (+Z)
    {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}},
    // Back (-Z)
    {{1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0}}};
// bottom-left, bottom-right, top-right, top-left