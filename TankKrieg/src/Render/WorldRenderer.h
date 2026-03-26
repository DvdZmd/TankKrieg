#pragma once

#include "Render/TankRenderer.h"

class Entity;
class World;
struct RenderContext;
class WorldVisualRegistry;

class WorldRenderer
{
public:
    void Render(const World& world, const RenderContext& renderContext, const WorldVisualRegistry& visualRegistry) const;

private:
    void RenderEntity(const Entity& entity, const RenderContext& renderContext, const WorldVisualRegistry& visualRegistry) const;

    TankRenderer tankRenderer;
};
