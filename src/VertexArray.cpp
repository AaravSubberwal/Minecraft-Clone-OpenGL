#include "VertexArray.h"
#include <glad/glad.h>
VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    vb.bind();
    const auto &elements = layout.getElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    }
}   