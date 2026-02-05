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

    // Converts a continuous direction in VISUAL space (screen-like: +x right, +y down)
    // into a continuous direction in ISO GRID space (gx, gy).
    // The output is normalized (unless input is near zero).
    inline Vector2 VisualDirToGridDir(const Vector2& visualDir, int tileW, int tileH)
    {
        // Safety
        const float lenSq = visualDir.x * visualDir.x + visualDir.y * visualDir.y;
        if (lenSq < 0.000001f) return { 0.0f, 0.0f };

        // From isometric projection:
        // sx = (gx - gy) * (tileW/2)
        // sy = (gx + gy) * (tileH/2)
        //
        // Solve for gx, gy (up to scale):
        // gx = 0.5 * ( sy/(tileH/2) + sx/(tileW/2) )
        // gy = 0.5 * ( sy/(tileH/2) - sx/(tileW/2) )

        const float a = tileW * 0.5f;
        const float b = tileH * 0.5f;

        // Treat visualDir as (sx, sy) direction
        const float gx = 0.5f * ((visualDir.y / b) + (visualDir.x / a));
        const float gy = 0.5f * ((visualDir.y / b) - (visualDir.x / a));

        Vector2 gridDir{ gx, gy };

        // Normalize output so speed isn't affected by diagonal magnitudes
        const float glenSq = gridDir.x * gridDir.x + gridDir.y * gridDir.y;
        if (glenSq > 0.000001f) {
            const float invLen = 1.0f / SDL_sqrtf(glenSq);
            gridDir.x *= invLen;
            gridDir.y *= invLen;
        }

        return gridDir;
    }

    // Snaps a direction to 8-way in GRID space.
    // Input should be a direction vector (any length). Output is {-1,0,1} in each axis,
    // normalized to length 1 for diagonals as well.
    inline Vector2 SnapGridDir8(const Vector2& gridDir, float deadzone = 0.2f)
    {
        const float lenSq = gridDir.x * gridDir.x + gridDir.y * gridDir.y;
        if (lenSq < deadzone * deadzone) return { 0.0f, 0.0f };

        int sx = (gridDir.x > 0.0f) ? 1 : (gridDir.x < 0.0f ? -1 : 0);
        int sy = (gridDir.y > 0.0f) ? 1 : (gridDir.y < 0.0f ? -1 : 0);

        Vector2 out{ (float)sx, (float)sy };

        // Normalize diagonal
        const float olenSq = out.x * out.x + out.y * out.y;
        if (olenSq > 0.000001f) {
            const float invLen = 1.0f / SDL_sqrtf(olenSq);
            out.x *= invLen;
            out.y *= invLen;
        }
        return out;
    }

}
