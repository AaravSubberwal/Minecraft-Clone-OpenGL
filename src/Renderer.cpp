#include "Renderer.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam)
{
    std::cerr << "OpenGL Debug Message (" << id << "): " << message << std::endl;

    if (severity == GL_DEBUG_SEVERITY_HIGH)
    {
        std::cerr << "Critical OpenGL Error! Application may crash!" << std::endl;
    }
}

void enableDebugging()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}
