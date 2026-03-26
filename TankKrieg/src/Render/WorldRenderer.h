#pragma once

#include "Render/EntityRenderDispatcher.h"
#include "Render/TankRenderer.h"

class Entity;
class World;
class WorldVisualRegistry;
struct RenderContext;

class WorldRenderer
{
public:
    void Render(const World& world, const RenderContext& renderContext, const WorldVisualRegistry& visualRegistry) const;

private:
    void RenderEntity(const Entity& entity, const RenderContext& renderContext, const WorldVisualRegistry& visualRegistry) const;

    TankRenderer tankRenderer;
    EntityRenderDispatcher entityRenderDispatcher{ tankRenderer };
};
