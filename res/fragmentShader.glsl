#version 460 core

in vec2 TexCoord;
flat in uint TexIndex; // Provided from the vertex shader

out vec4 FragColor;

uniform sampler2D u_atlas;
// Uniform for the grass tint. Adjust this to the green tint you want.
uniform vec3 u_grassTint;

void main()
{
    vec4 texColor = texture(u_atlas, TexCoord);
    
    // Apply tint if this is the grass-top texture.
    // For example, if your grass-top is at index 1.
    if (TexIndex == 0u)
    {
        texColor.rgb *= u_grassTint;
    }
    
    FragColor = texColor;
}
