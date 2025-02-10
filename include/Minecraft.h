#pragma once
#include <unordered_map>
#include <string>
#include <iostream>

class Block
{
private:
    static float vertices[96];
    static const unsigned int indices[36];
    static std::unordered_map<std::string, unsigned int> ID_Block_Map;

    float x, y, z;
    unsigned int ID;
    std::string type;

public:
    Block(std::string type);
    ~Block();
};

class WorldBlock
{
private:
    float x, y, z;
    unsigned short int light;

public:
    WorldBlock(std::string type, float x, float y, float z, unsigned short int light = 15);
    ~WorldBlock();
};

class chunk
{
private:

public:

};
