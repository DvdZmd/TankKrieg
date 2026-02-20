#include "Render/DebugOverlay.h"
#include "Math/IsoUtils.h"
#include "Render/IsoDebugDraw.h"

void DebugOverlay::Render(const RenderContext& ctx) const
{
    const SDL_FPoint cursorCenterPx = IsoUtils::GridToScreenF(
        static_cast<float>(cursorTileGrid.x),
        static_cast<float>(cursorTileGrid.y),
        ctx.tileWidthPx,
        ctx.tileHeightPx,
        ctx.originXPx,
        ctx.originYPx
    );

    SDL_SetRenderDrawColor(ctx.renderer, 255, 80, 80, 255);
    IsoDebugDraw::FillIsoDiamond(ctx.renderer, cursorCenterPx.x, cursorCenterPx.y, ctx.tileWidthPx, ctx.tileHeightPx);
    SDL_SetRenderDrawColor(ctx.renderer, 255, 80, 80, 255);
    IsoDebugDraw::DrawIsoDiamondOutline(ctx.renderer, cursorCenterPx.x, cursorCenterPx.y, ctx.tileWidthPx, ctx.tileHeightPx);

    const SDL_FPoint crosshairCenterPx = IsoUtils::GridToScreenF(
        crosshairGridTiles.x,
        crosshairGridTiles.y,
        ctx.tileWidthPx,
        ctx.tileHeightPx,
        ctx.originXPx,
        ctx.originYPx
    );

    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 0, 255);
    SDL_RenderLine(ctx.renderer, crosshairCenterPx.x - 5.0f, crosshairCenterPx.y, crosshairCenterPx.x + 5.0f, crosshairCenterPx.y);
    SDL_RenderLine(ctx.renderer, crosshairCenterPx.x, crosshairCenterPx.y - 5.0f, crosshairCenterPx.x, crosshairCenterPx.y + 5.0f);
}
