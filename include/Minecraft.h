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
    static const std::unordered_map<std::string, uint8_t> NameIDRegistery;
    static const std::unordered_map<uint8_t, TextData> IDTexRegistery;
//i need some sort of a hashmap to ID and what texture goes on what face
    std::string type;
    uint8_t ID;
public:
    Block(std::string type);
    ~Block();
};

struct TextData
{//each face of a block will have different U,V texture offsets to use this is unique for every ID and will stay const 
// 48 floats, 24 U,V coordinates per block
    float UV[48];  //back, front, left, right, bottom, top
};


class chunk
{
private:
    float xPos, yPos, zPos;
    uint8_t blockdata[16][128][16]; // 32KB lfg

public:
    void setFlat();
    void draw();
    void generateMesh();
};
