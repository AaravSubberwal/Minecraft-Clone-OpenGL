#include "textures.h"
#include "glad/glad.h"

Texture::Texture()
{
    glGenTextures(1, &ID);
}

Texture::~Texture()
{
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind(){
    glBindTexture(0, ID);
}