#version 460 core

// Attributes: position and texture atlas index.
layout (location = 0) in vec3 aPos;
layout (location = 1) in uint aTexIndex;

// Uniform transformation matrices.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

// Each cell is 1/16th of the atlas.
uniform float atlasCellSize; // Should be 1.0/16.0

// Pass the texture index (flat, so it doesn't interpolate) and UV coordinates.
flat out uint TexIndex;
out vec2 TexCoord;

void main()
{
    // Pass along the texture index.
    TexIndex = aTexIndex;
    
    // Determine base UV coordinates for the current quad vertex.
    // We assume each face (quad) has 4 vertices ordered as:
    // bottom-left, bottom-right, top-right, top-left.
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


    // Calculate the offset within the texture atlas.
    // With a cell size of 1/16, there are 16 cells per row.
    int cellsPerRow = int(1.0 / atlasCellSize); // Expected to be 16.
    int cellX = int(TexIndex) % cellsPerRow;
    int cellY = int(TexIndex) / cellsPerRow;
    vec2 offset = vec2(cellX, cellY) * atlasCellSize;

    // Final UV: offset plus the scaled base UV.
    TexCoord = offset + baseUV * atlasCellSize;

    // Standard transformation.
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}
