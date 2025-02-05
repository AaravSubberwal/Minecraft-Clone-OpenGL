#include "Renderer.h"

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
    glClear(GL_COLOR_BUFFER_BIT);
}
