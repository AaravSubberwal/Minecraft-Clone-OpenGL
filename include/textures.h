#pragma once

class Texture
{
private:
    unsigned int ID;
public:
    Texture();
    ~Texture();

    void bind();
    void unbind();
};