#pragma once
#include <SDL3/SDL.h>

namespace IsoDebugDraw
{
    /**
     * @brief Draw the outline of an isometric diamond tile.
     * @param r Renderer that receives the draw calls.
     * @param cx Screen-space X coordinate of the diamond center.
     * @param cy Screen-space Y coordinate of the diamond center.
     * @param tileW Tile width in pixels.
     * @param tileH Tile height in pixels.
     */
    void DrawIsoDiamondOutline(SDL_Renderer* r, float cx, float cy, int tileW, int tileH);
    /**
     * @brief Fill an isometric diamond tile with the default debug color.
     * @param r Renderer that receives the draw calls.
     * @param cx Screen-space X coordinate of the diamond center.
     * @param cy Screen-space Y coordinate of the diamond center.
     * @param tileW Tile width in pixels.
     * @param tileH Tile height in pixels.
     */
    void FillIsoDiamond(SDL_Renderer* r, float cx, float cy, int tileW, int tileH);
    /**
     * @brief Fill an isometric diamond tile using a caller-supplied color and scale.
     * @param r Renderer that receives the draw calls.
     * @param cx Screen-space X coordinate of the diamond center.
     * @param cy Screen-space Y coordinate of the diamond center.
     * @param tileW Tile width in pixels.
     * @param tileH Tile height in pixels.
     * @param color Fill color applied to the diamond geometry.
     * @param scale Scale factor applied around the diamond center.
     */
    void FillIsoDiamond(SDL_Renderer* r, float cx, float cy, int tileW, int tileH, SDL_FColor color, float scale = 1.0f);// NEW: configurable color + optional scale
}
