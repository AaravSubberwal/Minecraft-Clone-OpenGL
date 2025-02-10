#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <glad/glad.h>

#include "textures.h"
#include "Renderer.h"
#include "shader.h"


class Block
{
private:
    static float vertices[];
    static const unsigned int indices[];
    static std::unordered_map<std::string, uint8_t> ID_Block_Map;

    std::string type;
    uint8_t ID;
    static float frontV[];
    static float frontI[];
    static float leftV[];
    static float leftI[];
    static float rightV[];
    static float rightI[];

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
    float x, y, z;
    uint8_t blockdata[16][128][16]; // 32KB lfg

public:
    void drawFlat();
};


