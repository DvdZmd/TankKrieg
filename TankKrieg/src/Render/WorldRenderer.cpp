#include "Render/WorldRenderer.h"

#include "Entities/Entity.h"
#include "Entities/Tank.h"
#include "Render/RenderContext.h"
#include "Render/TankVisualDefinition.h"
#include "World/World.h"

void WorldRenderer::Render(const World& world, const RenderContext& renderContext, const Tank* playerTank, const TankVisualDefinition* playerTankVisual) const
{
    const std::vector<const Entity*> renderList = world.BuildRenderList();
    for (const Entity* entity : renderList)
    {
        if (entity != nullptr && entity->IsVisible())
        {
            RenderEntity(*entity, renderContext, playerTank, playerTankVisual);
        }
    }
}

void WorldRenderer::RenderEntity(const Entity& entity, const RenderContext& renderContext, const Tank* playerTank, const TankVisualDefinition* playerTankVisual) const
{
    if (const auto* tank = dynamic_cast<const Tank*>(&entity))
    {
        const TankVisualDefinition* tankVisual = (tank == playerTank) ? playerTankVisual : nullptr;
        tankRenderer.Render(tank->BuildVisualState(), renderContext, tankVisual);
    }
}
