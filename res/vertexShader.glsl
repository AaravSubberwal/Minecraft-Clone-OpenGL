#version 460 core
layout(location = 0) in vec3 aPos;      // Position attribute
layout(location = 1) in vec4 aColor;    // Color attribute

out vec4 ourColor; // Output color to fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}