#include "Render/IsoDebugDraw.h"

namespace IsoDebugDraw
{
    void FillIsoDiamond(SDL_Renderer* r, float cx, float cy, int tileW, int tileH, SDL_FColor color, float scale)
    {
        const float halfW = (tileW * 0.5f) * scale;
        const float halfH = (tileH * 0.5f) * scale;

        SDL_Vertex verts[4];

        SDL_FPoint top = { cx,        cy - halfH };
        SDL_FPoint right = { cx + halfW, cy };
        SDL_FPoint bottom = { cx,        cy + halfH };
        SDL_FPoint left = { cx - halfW, cy };

        for (int i = 0; i < 4; i++) {
            verts[i].color = color;
            verts[i].tex_coord = SDL_FPoint{ 0,0 };
        }

        verts[0].position = top;
        verts[1].position = right;
        verts[2].position = bottom;
        verts[3].position = left;

        int indices[6] = { 0,1,2, 0,2,3 };
        SDL_RenderGeometry(r, nullptr, verts, 4, indices, 6);
    }

    void FillIsoDiamond(SDL_Renderer* r, float cx, float cy, int tileW, int tileH)
    {
        SDL_Vertex verts[4];

        SDL_FPoint top = { cx,              cy - tileH * 0.5f };
        SDL_FPoint right = { cx + tileW * 0.5f, cy };
        SDL_FPoint bottom = { cx,              cy + tileH * 0.5f };
        SDL_FPoint left = { cx - tileW * 0.5f, cy };

        for (int i = 0; i < 4; i++) {
            verts[i].color = SDL_FColor{ 1.0f, 0.0f, 0.0f, 0.35f }; // semi-transparent fill
            verts[i].tex_coord = SDL_FPoint{ 0,0 };
        }

        verts[0].position = top;
        verts[1].position = right;
        verts[2].position = bottom;
        verts[3].position = left;

        int indices[6] = { 0,1,2,  0,2,3 };
        SDL_RenderGeometry(r, nullptr, verts, 4, indices, 6);
    }

    void DrawIsoDiamondOutline(SDL_Renderer* r, float cx, float cy, int tileW, int tileH)
    {
        SDL_FPoint top = { cx,              cy - tileH * 0.5f };
        SDL_FPoint right = { cx + tileW * 0.5f, cy };
        SDL_FPoint bottom = { cx,              cy + tileH * 0.5f };
        SDL_FPoint left = { cx - tileW * 0.5f, cy };

        SDL_RenderLine(r, top.x, top.y, right.x, right.y);
        SDL_RenderLine(r, right.x, right.y, bottom.x, bottom.y);
        SDL_RenderLine(r, bottom.x, bottom.y, left.x, left.y);
        SDL_RenderLine(r, left.x, left.y, top.x, top.y);
    }
}
