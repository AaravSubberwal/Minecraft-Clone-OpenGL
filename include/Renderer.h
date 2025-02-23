#pragma once
#include "shader.h"
#include <iostream>

void enableDebugging();
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam);

struct Vertex
{
    glm::ivec3 position;
    uint8_t texIndex;
};

namespace std
{
    template <>
    struct hash<glm::ivec2>
    {
        size_t operator()(const glm::ivec2 &pos) const
        {
            size_t h1 = std::hash<int>{}(pos.x);
            size_t h3 = std::hash<int>{}(pos.y);
            return h1 ^ (h3 << 2);
        }
    };
}
