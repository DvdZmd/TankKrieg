#include "World/World.h"

/**
 * @brief Add an entity to the world while preserving unique ownership.
 * @param entity Entity instance transferred into world ownership.
 * @return A raw pointer to the stored entity, or nullptr when the input is null.
 */
Entity* World::Add(std::unique_ptr<Entity> entity)
{
    if (!entity)
        return nullptr;

    Entity* raw = entity.get();
    entities.push_back(std::move(entity));
    return raw;
}

/**
 * @brief Update all entities currently contained in the world.
 * @param dt Elapsed time in seconds since the previous update.
 */
void World::Update(float dt)
{
    for (auto& e : entities)
    {
        // In the future: skip dead entities, or handle pending removals
        e->Update(dt);
    }
}

/**
 * @brief Render all entities currently contained in the world.
 * @param ctx Rendering data shared across the current frame.
 */
void World::Render(const RenderContext& ctx) const
{
    for (const auto& e : entities)
    {
        e->Render(ctx);
    }
}

/**
 * @brief Remove every entity owned by the world.
 */
void World::Clear()
{
    entities.clear();
}
