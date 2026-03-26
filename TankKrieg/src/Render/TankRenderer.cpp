#include "Render/TankRenderer.h"

#include "Math/IsoUtils.h"
#include "Render/TankDirectionResolver.h"
#include "Render/TankDebugRenderer.h"
#include "Render/TankVisualDefinition.h"

namespace
{
    void RenderLayer(const DirectionalSpriteSet& layer, const Vector2& worldAnchor, float radians, const RenderContext& ctx)
    {
        if (!layer.IsValid())
        {
            return;
        }

        const SDL_FPoint screenAnchor = IsoUtils::GridToScreenF(
            worldAnchor.x,
            worldAnchor.y,
            ctx.tileWidthPx,
            ctx.tileHeightPx,
            ctx.originXPx,
            ctx.originYPx);

        const int directionIndex = TankDirectionResolver::AngleToDirectionIndex(radians);
        const SDL_FRect destinationRect{
            screenAnchor.x - layer.originPx.x,
            screenAnchor.y - layer.originPx.y,
            layer.sizePx.x,
            layer.sizePx.y
        };
        const SDL_FRect& sourceRect = layer.sourceRects[directionIndex];

        SDL_RenderTexture(ctx.renderer, layer.texture, &sourceRect, &destinationRect);
    }
}

void TankRenderer::Render(const TankVisualState& tankVisualState, const RenderContext& ctx, const TankVisualDefinition* visualDefinition) const
{
    if (visualDefinition != nullptr && visualDefinition->IsComplete())
    {
        RenderLayer(visualDefinition->hull, tankVisualState.worldPosition, tankVisualState.hullAngleRadians, ctx);
        RenderLayer(visualDefinition->turret, tankVisualState.worldPosition, tankVisualState.turretAngleRadians, ctx);
        return;
    }

    TankDebugRenderer::Render(tankVisualState, ctx);
}
