#include "textures.h"
#include "glad/glad.h"
#include "stb_image.h"

Texture::Texture(const std::string &path)
    : filepath(path),
      localbuffer(nullptr), ID(0), width(0), height(0), BPP(0) // Added BPP initialization
{
    // stbi_set_flip_vertically_on_load(1);
    localbuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (!localbuffer)
    {
        printf("Failed to load texture: %s\n", path.c_str());
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localbuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(localbuffer);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind for safety
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}