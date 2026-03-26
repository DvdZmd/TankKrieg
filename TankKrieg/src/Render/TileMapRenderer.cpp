#include "Render/TileMapRenderer.h"
#include "Math/IsoUtils.h"
#include "Render/IsoDebugDraw.h"

/**
 * @brief Render filled tiles and their outlines for the current map.
 * @param ctx Rendering data shared across the current frame.
 * @param map Tile map that defines the visible grid bounds.
 */
void TileMapRenderer::Render(const RenderContext& ctx, const TileMap& map) const
{
    const SDL_FColor tileFillColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    const SDL_FColor blockedFillColor{ 0.75f, 0.45f, 0.45f, 1.0f };

    for (int gy = 0; gy < map.Height(); ++gy)
    {
        for (int gx = 0; gx < map.Width(); ++gx)
        {
            const Int2 tile{ gx, gy };
            const SDL_FPoint p = IsoUtils::GridToScreenF(
                (float)gx, (float)gy,
                ctx.tileWidthPx, ctx.tileHeightPx,
                ctx.originXPx, ctx.originYPx
            );
            const SDL_FColor& fillColor = map.BlocksMovement(tile) ? blockedFillColor : tileFillColor;
            IsoDebugDraw::FillIsoDiamond(ctx.renderer, p.x, p.y, ctx.tileWidthPx, ctx.tileHeightPx, fillColor, 1.0f);
        }
    }

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
