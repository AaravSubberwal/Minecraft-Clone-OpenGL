#include "UI.h"

UI::UI() : shader2D("C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/2dVertexShader.glsl", "C:/Users/Aarav/Desktop/Projects/Minecraft-Clone-OpenGL/res/2dFragmentShader.glsl")
{
    cookCrosshair();
}

UI::~UI()
{
    glDeleteVertexArrays(1, &crosshairVAO);
    glDeleteBuffers(1, &crosshairVBO);
}

void UI::slapUI()
{
    shader2D.bind();
    glDisable(GL_DEPTH_TEST); // draw on top

    glBindVertexArray(crosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}

void UI::cookCrosshair()
{
    float crosshairLines[] = {
        // horizontal line
        -0.015f, 0.0f,
        0.015f, 0.0f,
        // vertical line
        0.0f, -0.02f,
        0.0f, 0.02f};
    glGenVertexArrays(1, &crosshairVAO);
    glGenBuffers(1, &crosshairVBO);

    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairLines), crosshairLines, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glLineWidth(2.0f);

    glBindVertexArray(0);
}
