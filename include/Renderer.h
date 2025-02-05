#include <glad/glad.h>

#include "GLbuffers.h"
#include "shader.h"

class Renderer
{
private:
public:
    void draw(const IndexBuffer &ib, const VertexArray &va, const Shader &shader) const;
    void backColor(float v0, float v1, float v2, float v3) const;
    void clear() const;
};
