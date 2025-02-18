#version 460 core

layout (location = 0) in ivec3 v_Position; // 16-bit integer position
layout (location = 1) in uint v_TexIndex;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

flat out uint f_TexIndex;
out vec2 UVcoord;

void main()
{
    f_TexIndex = v_TexIndex;

    int vertexID = gl_VertexID % 4;
    vec2 baseUV;
    if (vertexID == 0)
        baseUV = vec2(0.0, 1.0); // top-left
    else if (vertexID == 1)
        baseUV = vec2(1.0, 1.0); // top-right
    else if (vertexID == 2)
        baseUV = vec2(1.0, 0.0); // bottom-right
    else // vertexID == 3
        baseUV = vec2(0.0, 0.0); // bottom-left

    // Compute atlas UV offset
    int cellsPerRow = int(1.0 / 0.0625f);
    ivec2 cellIndex = ivec2(int(f_TexIndex) % cellsPerRow, int(f_TexIndex) / cellsPerRow);
    vec2 offset = vec2(cellIndex) * 0.0625f;

    UVcoord = offset + baseUV * 0.0625f;

    // Convert integer position to float before transformation
    gl_Position = u_projection * u_view * u_model * vec4(vec3(v_Position), 1.0);
}
