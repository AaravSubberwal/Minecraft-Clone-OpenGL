#version 460 core
layout(location = 0) out vec4 color;

in vec2 texCoords;
uniform sampler2D u_grass;

void main()
{
    color = texture(u_grass, texCoords);
}
