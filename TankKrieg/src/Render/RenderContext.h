#pragma once

#include <SDL3/SDL.h>

struct RenderContext
{
    SDL_Renderer* renderer = nullptr;
    int tileW = 0;
    int tileH = 0;
    int originX = 0;
    int originY = 0;
};
