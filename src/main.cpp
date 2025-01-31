/**************************************************************************************************************************** 
Author: Aarav Subberwal
Date: 

Cloning Minecraft using OpenGL 4.6, GLAD, GLFW and C++
Work In Progress
currently no way to build except using vscode. I'll add CMake later.
******************************************************************************************************************************/
#include <iostream>
#include <string>
#include <utility>
#include <cstdio>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
using namespace std;

std::pair<std::string, std::string> readShadersFromFile(const std::string &filename)
{
    FILE *file = fopen(filename.c_str(), "rb");
    if (!file)
    {
        std::cerr << "Error: Could not open the file: " << filename << std::endl;
        return {"", ""};
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = new char[fileSize + 1];
    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    std::string fileContent(buffer);
    delete[] buffer;

    size_t vertexStart = fileContent.find("#vertex");
    size_t fragmentStart = fileContent.find("#fragment");

    if (vertexStart == std::string::npos || fragmentStart == std::string::npos)
    {
        std::cerr << "Error: Shader file is missing #vertex or #fragment delimiter." << std::endl;
        return {"", ""};
    }

    vertexStart += strlen("#vertex\n");
    std::string vertexShader = fileContent.substr(vertexStart, fragmentStart - vertexStart);

    fragmentStart += strlen("#fragment\n");
    std::string fragmentShader = fileContent.substr(fragmentStart);

    return {vertexShader, fragmentShader};
}

static unsigned int CompileShader(unsigned int type, const string &source)
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
        cout << "Failed to compile shader:" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << endl;
        cout << message << "\n";
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const string &vertexShader, const string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Check for linking errors
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(800, 600, "Aarav", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    {
        float positions[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0};

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        VertexBuffer vb(positions, sizeof(positions));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

        IndexBuffer ib(indices, 6);

        auto [vertexShader, fragmentShader] = readShadersFromFile("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/shaders.glsl");
        if (vertexShader.empty() || fragmentShader.empty())
        {
            std::cerr << "Failed to load shaders!" << std::endl;
            return -1;
        }
        // std::cout << "Vertex Shader:\n"
        //           << vertexShader << "\n";
        // std::cout << "Fragment Shader:\n"
        //           << fragmentShader << "\n";

        unsigned int shader_program = CreateShader(vertexShader, fragmentShader);
        if (shader_program == 0)
        {
            std::cerr << "Failed to create shader program!" << std::endl;
            return -1;
        }
        glUseProgram(shader_program);

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(vao);
            ib.bind();
            glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteProgram(shader_program);
        glDeleteVertexArrays(1, &vao);
    }
    glfwTerminate();
    return 0;
}