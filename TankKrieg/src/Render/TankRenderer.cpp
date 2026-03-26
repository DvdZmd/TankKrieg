#include "Render/TankRenderer.h"

#include "Render/TankDebugRenderer.h"
#include "Render/TankVisual.h"

void TankRenderer::Render(const TankRenderData& tankRenderData, const RenderContext& ctx, const TankVisual* visual) const
{
    if (TankVisual::IsComplete(visual))
    {
        TankVisual::RenderLayer(visual->hull, tankRenderData.worldPosition, tankRenderData.hullAngleRadians, ctx);
        TankVisual::RenderLayer(visual->turret, tankRenderData.worldPosition, tankRenderData.turretAngleRadians, ctx);
        return;
    }

    TankDebugRenderer::Render(tankRenderData, ctx);
}
