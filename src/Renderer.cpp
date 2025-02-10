#include "Renderer.h"
#include <iostream>

void Renderer::draw(const IndexBuffer &ib, const VertexArray &va, const Shader &shader) const
{
    ib.bind();
    va.bind();
    shader.bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::backColor(float v0, float v1, float v2, float v3) const
{
    glClearColor(v0, v1, v2, v3);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam)
{
    std::cerr << "OpenGL Debug Message (" << id << "): " << message << std::endl;

    if (severity == GL_DEBUG_SEVERITY_HIGH)
    {
        std::cerr << "Critical OpenGL Error! Application may crash!" << std::endl;
    }
}

void enableDebugging()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}
