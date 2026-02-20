#include "Render/TileMapRenderer.h"
#include "Math/IsoUtils.h"
#include "Render/IsoDebugDraw.h"

void TileMapRenderer::Render(const RenderContext& ctx, const TileMap& map) const
{
    const SDL_FColor tileFillColor{ 1.0f, 1.0f, 1.0f, 1.0f };

    // First pass: fill all tiles in white.
    for (int gy = 0; gy < map.Height(); ++gy)
    {
        for (int gx = 0; gx < map.Width(); ++gx)
        {
            const SDL_FPoint p = IsoUtils::GridToScreenF(
                (float)gx, (float)gy,
                ctx.tileWidthPx, ctx.tileHeightPx,
                ctx.originXPx, ctx.originYPx
            );
            IsoDebugDraw::FillIsoDiamond(ctx.renderer, p.x, p.y, ctx.tileWidthPx, ctx.tileHeightPx, tileFillColor, 1.0f);
        }
    }

    // Second pass: draw edges in dark gray.
    SDL_SetRenderDrawColor(ctx.renderer, 80, 80, 80, 255);
    for (int gy = 0; gy < map.Height(); ++gy)
    {
        for (int gx = 0; gx < map.Width(); ++gx)
        {
            const SDL_FPoint p = IsoUtils::GridToScreenF(
                (float)gx, (float)gy,
                ctx.tileWidthPx, ctx.tileHeightPx,
                ctx.originXPx, ctx.originYPx
            );
            IsoDebugDraw::DrawIsoDiamondOutline(ctx.renderer, p.x, p.y, ctx.tileWidthPx, ctx.tileHeightPx);
        }
    }
}
