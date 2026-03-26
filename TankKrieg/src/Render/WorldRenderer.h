#pragma once

#include "Render/TankRenderer.h"

class Entity;
class Tank;
struct TankVisualDefinition;
class World;
struct RenderContext;

class WorldRenderer
{
public:
    void Render(const World& world, const RenderContext& renderContext, const Tank* playerTank, const TankVisualDefinition* playerTankVisual) const;

private:
    void RenderEntity(const Entity& entity, const RenderContext& renderContext, const Tank* playerTank, const TankVisualDefinition* playerTankVisual) const;

    TankRenderer tankRenderer;
};
