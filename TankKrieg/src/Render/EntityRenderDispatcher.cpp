#include "Render/EntityRenderDispatcher.h"

#include "Entities/Entity.h"
#include "Entities/Tank.h"
#include "Render/TankRenderer.h"
#include "Render/WorldVisualRegistry.h"

EntityRenderDispatcher::EntityRenderDispatcher(const TankRenderer& tankRendererRef)
    : tankRenderer(tankRendererRef)
{
}

void EntityRenderDispatcher::Dispatch(const Entity& entity, const RenderContext& renderContext, const WorldVisualRegistry& visualRegistry) const
{
    if (const auto* tank = dynamic_cast<const Tank*>(&entity))
    {
        RenderTank(*tank, renderContext, visualRegistry.Resolve(entity));
    }
}

void EntityRenderDispatcher::RenderTank(const Tank& tank, const RenderContext& renderContext, const WorldVisualDefinitionBinding& binding) const
{
    tankRenderer.Render(tank.BuildVisualState(), renderContext, binding.GetTank());
}
