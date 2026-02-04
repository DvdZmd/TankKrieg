#pragma once
#include <SDL3/SDL.h>
#include "Math/Int2.h"
#include "Math/Vector2.h"

namespace IsoUtils
{
    // Converts 2D "visual screen directions" (x right/left, y down/up)
    // into isometric grid step (gx, gy).
    inline Int2 VisualToIsoGridStep(const Int2& v)
    {
        // 4-dir (visual)
        if (v.x == 0 && v.y == -1) return { -1, -1 }; // up
        if (v.x == 0 && v.y == 1) return { 1,  1 }; // down
        if (v.x == -1 && v.y == 0) return { -1,  1 }; // left
        if (v.x == 1 && v.y == 0) return { 1, -1 }; // right

        // 8-dir diagonals (visual)
        if (v.x == -1 && v.y == -1) return { -1,  0 }; // up-left
        if (v.x == 1 && v.y == -1) return { 0, -1 }; // up-right
        if (v.x == -1 && v.y == 1) return { 0,  1 }; // down-left
        if (v.x == 1 && v.y == 1) return { 1,  0 }; // down-right

        return { 0, 0 };
    }

    inline SDL_FPoint GridToScreenF(float gx, float gy, int tileW, int tileH, int originX, int originY)
    {
        const float sx = originX + (gx - gy) * (tileW * 0.5f);
        const float sy = originY + (gx + gy) * (tileH * 0.5f);
        return { sx, sy };
    }

    inline SDL_FPoint GridToScreen(int gx, int gy, int tileW, int tileH, int originX, int originY)
    {
        return GridToScreenF((float)gx, (float)gy, tileW, tileH, originX, originY);
    }
}
