#include "Minecraft.h"

int main()
{
    World world;
    while (!world.shouldclose())
    {
        world.render();
    }
}