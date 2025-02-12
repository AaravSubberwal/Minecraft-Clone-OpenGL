#include "GLbuffers.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size, unsigned int hint)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, hint);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &ID);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
void VertexBufferLayout::push<float>(unsigned int count)
{
    Elements.push_back({GL_FLOAT, count, GL_FALSE});
    stride += count * sizeof(GLfloat);
}

template <>
void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    stride += count * sizeof(GLuint);
}

template <>
void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    stride += count * sizeof(GLubyte);
}

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
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void *)(intptr_t)offset);
        offset += element.count * VertexBufferElement::getSizeofType(element.type);
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

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count, unsigned int hint)
: count(count)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, hint);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &ID);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

