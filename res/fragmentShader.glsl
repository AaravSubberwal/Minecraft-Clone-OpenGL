#version 460 core

in vec2 UVcoord;
flat in uint f_TexIndex; 

out vec4 FragColor;

uniform sampler2D u_atlas;
uniform vec3 u_grassTint;

void main()
{
    vec4 texColor = texture(u_atlas, UVcoord);
    
    if (f_TexIndex == 0u)
    {
        texColor.rgb *= u_grassTint;
    }
    
    FragColor = texColor;
}
