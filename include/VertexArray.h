#pragma once
#include "VertexBuffer.h"

class VertexArray{
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};