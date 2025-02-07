#version 460 core
in vec4 ourColor;       // Input color from vertex shader
out vec4 FragColor;     // Output color

void main()
{
    FragColor = ourColor;
}