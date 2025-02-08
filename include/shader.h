#pragma once
#include <string>

class Shader
{
private:
    unsigned int ID;

public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void bind() const;
    void unbind() const;

    unsigned int compileShader(unsigned int type, const std::string &source);
    unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader);

    // Set uniforms
    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void setUniform1i(const std::string &name, int value);


private:
    unsigned int getUniformLocations(const std::string &name);
    std::string readFromFile(const std::string &path);
};
