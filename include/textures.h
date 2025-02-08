#pragma once
#include <string>
class Texture
{
private:
    unsigned int ID;
    std::string filepath;
    unsigned char* localbuffer;
    int width, height, BPP;
public:
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const {return width;}
    inline int getHeight() const {return height;}
};