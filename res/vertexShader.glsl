#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

out vec2 texCoords;
uniform mat4 u_model;

void main()
{
    texCoords = aTex;
    gl_Position = u_model * vec4(aPos, 1.0);
}
