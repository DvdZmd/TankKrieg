#include "World/World.h"

Entity* World::Add(std::unique_ptr<Entity> entity)
{
    if (!entity)
        return nullptr;

    Entity* raw = entity.get();
    entities.push_back(std::move(entity));
    return raw;
}

void World::Update(float dt)
{
    for (auto& e : entities)
    {
        // In the future: skip dead entities, or handle pending removals
        e->Update(dt);
    }
}

void World::Render(const RenderContext& ctx) const
{
    for (const auto& e : entities)
    {
        e->Render(ctx);
    }
}

void World::Clear()
{
    entities.clear();
}
