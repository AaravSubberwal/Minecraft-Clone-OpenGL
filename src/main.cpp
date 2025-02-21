#include "Minecraft.h"

int main()
{
    World world;
    while (!world.shouldClose())
    {
        world.render();
    }
}