#pragma once
#include <vector>
#include <glad/glad.h>

class VertexBuffer
{
public:
    VertexBuffer(const void *data, unsigned int size, unsigned int hint);
    ~VertexBuffer();

    void bind() const;

    void unbind() const;

private:
    unsigned int ID;
};

class VertexBufferElement
{
public:
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeofType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> Elements;
    unsigned int stride;

public:
    VertexBufferLayout() : stride(0) {}
    ~VertexBufferLayout() = default;

    template <typename T>
    void push(unsigned int count);

    const std::vector<VertexBufferElement> &getElements() const { return Elements; }
    unsigned int getStride() const { return stride; }

    void clear()
    {
        Elements.clear();
        stride = 0;
    }
};

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    unsigned int getCount() const { return count; }

private:
    unsigned int ID;
    unsigned int count;
};

class VertexArray
{
private:
    unsigned int ID;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void bind() const;
    void unbind() const;
    unsigned int getID() const;
};