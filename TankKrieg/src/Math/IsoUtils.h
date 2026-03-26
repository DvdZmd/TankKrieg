#pragma once
#include <SDL3/SDL.h>
#include "Math/Int2.h"
#include "Math/Vector2.h"

namespace IsoUtils
{
    /**
     * @brief Map a visual input direction to a discrete isometric grid step.
     * @param v Direction in screen-like space where positive X moves right and positive Y moves down.
     * @return The corresponding grid-space step, or { 0, 0 } when the direction is unsupported.
     */
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

    /**
     * @brief Project a floating-point grid position into isometric screen space.
     * @param gx Grid X coordinate in tile space.
     * @param gy Grid Y coordinate in tile space.
     * @param tileWidthPx Width of one tile in pixels.
     * @param tileHeightPx Height of one tile in pixels.
     * @param originXPx Screen-space X origin used for the projection.
     * @param originYPx Screen-space Y origin used for the projection.
     * @return The projected screen-space position in pixels.
     */
    inline SDL_FPoint GridToScreenF(float gx, float gy, int tileWidthPx, int tileHeightPx, int originXPx, int originYPx)
    {
        const float sx = originXPx + (gx - gy) * (tileWidthPx * 0.5f);
        const float sy = originYPx + (gx + gy) * (tileHeightPx * 0.5f);
        return { sx, sy };
    }

    /**
     * @brief Project an integer grid position into isometric screen space.
     * @param gx Grid X coordinate in tile space.
     * @param gy Grid Y coordinate in tile space.
     * @param tileWidthPx Width of one tile in pixels.
     * @param tileHeightPx Height of one tile in pixels.
     * @param originXPx Screen-space X origin used for the projection.
     * @param originYPx Screen-space Y origin used for the projection.
     * @return The projected screen-space position in pixels.
     */
    inline SDL_FPoint GridToScreen(int gx, int gy, int tileWidthPx, int tileHeightPx, int originXPx, int originYPx)
    {
        return GridToScreenF((float)gx, (float)gy, tileWidthPx, tileHeightPx, originXPx, originYPx);
    }

    /**
     * @brief Convert a continuous visual direction into normalized grid-space movement.
     * @param visualDir Direction in screen-like space with arbitrary magnitude.
     * @param tileWidthPx Tile width in pixels used by the isometric projection.
     * @param tileHeightPx Tile height in pixels used by the isometric projection.
     * @return The normalized grid-space direction, or { 0, 0 } when the input is near zero.
     */
    inline Vector2 VisualDirToGridDir(const Vector2& visualDir, int tileWidthPx, int tileHeightPx)
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

        const float a = tileWidthPx * 0.5f;
        const float b = tileHeightPx * 0.5f;

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

    /**
     * @brief Quantize a grid-space direction to one of eight movement directions.
     * @param gridDir Direction in grid space with any magnitude.
     * @param deadzone Minimum input magnitude required before a direction is emitted.
     * @return The snapped direction, or { 0, 0 } when the input falls inside the deadzone.
     */
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
