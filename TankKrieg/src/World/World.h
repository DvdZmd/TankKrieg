#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "Render/RenderContext.h"
#include "Entities/Entity.h"

class World
{
public:
    /**
     * @brief Construct an empty world container.
     */
    World() = default;
    /**
     * @brief Destroy the world and any owned entities.
     */
    ~World() = default;

    // Non-copyable (unique_ptr ownership)
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    // Movable
    World(World&&) noexcept = default;
    World& operator=(World&&) noexcept = default;

    /**
     * @brief Add a newly created entity to the world.
     * @param entity Entity instance transferred into world ownership.
     * @return A raw pointer to the stored entity, or nullptr when the input is null.
     */
    Entity* Add(std::unique_ptr<Entity> entity);

    /**
     * @brief Update every entity currently stored in the world.
     * @param dt Elapsed time in seconds since the previous update.
     */
    void Update(float dt);

    /**
     * @brief Render every entity currently stored in the world.
     * @param ctx Rendering data shared across the current frame.
     */
    void Render(const RenderContext& ctx) const;

    /**
     * @brief Remove all entities from the world.
     */
    void Clear();

    /**
     * @brief Return the number of entities currently owned by the world.
     * @return The number of stored entities.
     */
    std::size_t Count() const { return entities.size(); }

private:
    std::vector<std::unique_ptr<Entity>> entities;
};
