#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size,unsigned int hint)
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
