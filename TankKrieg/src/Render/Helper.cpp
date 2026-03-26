#pragma once
#include <SDL3/SDL.h>
#include "Render/Helper.h"
#include "Math/Vector2.h"
#include <cmath>

namespace RenderHelper
{
    /**
     * @brief Draw a thick line segment by expanding it into a rendered quad.
     * @param renderer Renderer that receives the draw calls.
     * @param a Start point of the line segment.
     * @param b End point of the line segment.
     * @param thickness Line thickness in pixels.
     * @param color Color applied to the generated geometry.
     */
    void DrawThickLine(
        SDL_Renderer* renderer,
        const SDL_FPoint& a,
        const SDL_FPoint& b,
        float thickness,
        const SDL_FColor& color)
    {
        Vector2 dir{ b.x - a.x, b.y - a.y };
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len <= 0.0001f) return;

        dir.x /= len;
        dir.y /= len;

        // Perpendicular vector
        Vector2 perp{ -dir.y, dir.x };
        float half = thickness * 0.5f;

        SDL_FPoint v0{ a.x + perp.x * half, a.y + perp.y * half };
        SDL_FPoint v1{ b.x + perp.x * half, b.y + perp.y * half };
        SDL_FPoint v2{ b.x - perp.x * half, b.y - perp.y * half };
        SDL_FPoint v3{ a.x - perp.x * half, a.y - perp.y * half };

        SDL_Vertex verts[4];

        for (int i = 0; i < 4; ++i)
            verts[i].color = color;

        verts[0].position = v0;
        verts[1].position = v1;
        verts[2].position = v2;
        verts[3].position = v3;

        int indices[6] = { 0,1,2, 2,3,0 };

        SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);
    }

    /**
     * @brief Draw a triangle outline by rendering three thick line segments.
     * @param renderer Renderer that receives the draw calls.
     * @param a First triangle vertex.
     * @param b Second triangle vertex.
     * @param c Third triangle vertex.
     * @param thickness Outline thickness in pixels.
     * @param color Color applied to the generated geometry.
     */
    void DrawTriangleThick(
        SDL_Renderer* renderer,
        const SDL_FPoint& a,
        const SDL_FPoint& b,
        const SDL_FPoint& c,
        float thickness,
        const SDL_FColor& color)
    {
        DrawThickLine(renderer, a, b, thickness, color);
        DrawThickLine(renderer, b, c, thickness, color);
        DrawThickLine(renderer, c, a, thickness, color);
    }
}
