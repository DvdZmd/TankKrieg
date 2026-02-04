#pragma once
#include <SDL3/SDL.h>

namespace IsoDebugDraw
{
    void DrawIsoDiamondOutline(SDL_Renderer* r, float cx, float cy, int tileW, int tileH);
    void FillIsoDiamond(SDL_Renderer* r, float cx, float cy, int tileW, int tileH);
}
