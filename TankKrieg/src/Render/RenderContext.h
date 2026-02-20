#pragma once

#include <SDL3/SDL.h>

struct RenderContext
{
    SDL_Renderer* renderer = nullptr;
    int tileWidthPx = 0;
    int tileHeightPx = 0;
    int originXPx = 0;
    int originYPx = 0;
};
