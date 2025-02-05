#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &ID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &ID);
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    bind();
    vb.bind();
    const auto &elements = layout.getElements();
    unsigned int offset=0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)(intptr_t)offset);
        offset+=element.count*VertexBufferElement::getSizeofType(element.type);
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(ID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

unsigned int VertexArray::getID() const
{
    return ID;
}
