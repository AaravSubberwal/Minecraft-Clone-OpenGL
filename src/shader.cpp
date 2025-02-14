#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.h"

using namespace std;

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexShader = readFromFile(vertexPath);
    std::string fragmentShader = readFromFile(fragmentPath);
    ID = createShader(vertexShader, fragmentShader);
    if (ID == 0)
    {
        std::cerr << "Failed to create shader program!" << std::endl;
    }
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::bind() const
{
    glUseProgram(ID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

std::string Shader::readFromFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::createShader(const string &vertexShader, const string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << endl;
        cout << message << "\n";
        glDeleteShader(id);
        return 0;
    }
    return id;
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    bind();
    glUniform4f(getUniformLocations(name), v0, v1, v2, v3);
}

void Shader::setUniform1i(const std::string &name, int value)
{
    bind();
    glUniform1i(getUniformLocations(name), value);
}

void Shader::setUniformMatrix4fv(const std::string &name, glm::mat4 mat,  bool transpose)
{
    bind();
    glUniformMatrix4fv(getUniformLocations(name), 1, transpose, glm::value_ptr(mat));
}

void Shader::setUniform1f(const std::string &name, float value)
{
    bind();
    glUniform1f(getUniformLocations(name), value);
}

void Shader::setUniform3f(const std::string &name, float v0, float v1, float v2)
{    bind();
    glUniform3f(getUniformLocations(name), v0, v1, v2);
}

unsigned int Shader::getUniformLocations(const std::string &name)
{
    bind();
    return glGetUniformLocation(ID, name.c_str());
}
