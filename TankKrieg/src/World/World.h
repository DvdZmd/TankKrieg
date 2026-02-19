#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "Render/RenderContext.h"
#include "Entities/Entity.h"

class World
{
public:
    World() = default;
    ~World() = default;

    // Non-copyable (unique_ptr ownership)
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    // Movable
    World(World&&) noexcept = default;
    World& operator=(World&&) noexcept = default;

    // Adds an entity to the world and returns a raw pointer for convenience.
    // The World remains the owner.
    Entity* Add(std::unique_ptr<Entity> entity);

    // Calls Update on all entities.
    void Update(float dt);

    // Calls Render on all entities.
    // In a later step we will add proper isometric depth sorting here.
    void Render(const RenderContext& ctx) const;

    // Basic lifecycle
    void Clear();

    // Accessors (useful for debugging / tools)
    std::size_t Count() const { return entities.size(); }

private:
    std::vector<std::unique_ptr<Entity>> entities;
};
