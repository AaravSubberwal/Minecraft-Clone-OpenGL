#pragma once
#include <vector>
class VertexBuffer
{
public:
    VertexBuffer(const void *data, unsigned int size);
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
    bool normalized;
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> Elements;
    unsigned int stride;

public:
    VertexBufferLayout() : stride(0) {}
    ~VertexBufferLayout() = default;

    // Generic template definition (will fail for unsupported types)
    template <typename T>
    void push(int count)
    {
        static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::push");
    }

    template <>
    void push<float>(int count)
    {
        Elements.push_back({GL_FLOAT, static_cast<unsigned int>(count), false});
        stride += count * sizeof(GLfloat);
    }

    template <>
    void push<unsigned int>(int count)
    {
        Elements.push_back({GL_UNSIGNED_INT, static_cast<unsigned int>(count), false});
        stride += count * sizeof(GLuint);
    }
    const std::vector<VertexBufferElement>& getElements() const { return Elements; }
    unsigned int getStride() const { return stride; }
};