#pragma once
#include "shader.h"
#include <iostream>


void enableDebugging();
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar *message, const void *userParam);