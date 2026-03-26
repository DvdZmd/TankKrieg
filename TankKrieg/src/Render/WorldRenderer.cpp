#include "Render/WorldRenderer.h"

#include "Entities/Entity.h"
#include "Render/RenderContext.h"
#include "Render/WorldVisualRegistry.h"
#include "World/World.h"

void WorldRenderer::Render(const World& world, const RenderContext& renderContext, const WorldVisualRegistry& visualRegistry) const
{
    const std::vector<const Entity*> renderList = world.BuildRenderList();
    for (const Entity* entity : renderList)
    {
        if (entity != nullptr && entity->IsVisible())
        {
            RenderEntity(*entity, renderContext, visualRegistry);
        }
    }
}

void WorldRenderer::RenderEntity(const Entity& entity, const RenderContext& renderContext, const WorldVisualRegistry& visualRegistry) const
{
    entityRenderDispatcher.Dispatch(entity, renderContext, visualRegistry);
}
