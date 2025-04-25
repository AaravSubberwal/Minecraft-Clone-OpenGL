#include "shader.h"

class UI
{
private:
    Shader shader2D;
    unsigned int crosshairVAO, crosshairVBO;

public:
    UI();
    ~UI();

    void slapUI();
    void cookCrosshair();
};