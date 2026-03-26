#pragma once

class Entity;
class Tank;
struct RenderContext;
class TankRenderer;
class WorldVisualRegistry;
class WorldVisualDefinitionBinding;

class EntityRenderDispatcher
{
public:
    explicit EntityRenderDispatcher(const TankRenderer& tankRenderer);

    void Dispatch(const Entity& entity, const RenderContext& renderContext, const WorldVisualRegistry& visualRegistry) const;

private:
    void RenderTank(const Tank& tank, const RenderContext& renderContext, const WorldVisualDefinitionBinding& binding) const;

    const TankRenderer& tankRenderer;
};
