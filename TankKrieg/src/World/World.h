#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

#include "Entities/Entity.h"
#include "Render/RenderContext.h"

class World
{
public:
    /**
     * @brief Construct an empty gameplay world.
     */
    World() = default;

    /**
     * @brief Destroy the world and any owned entities.
     */
    ~World() = default;

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    World(World&&) noexcept = default;
    World& operator=(World&&) noexcept = default;

    /**
     * @brief Add a newly created entity to the world.
     * @param entity Entity instance transferred into world ownership.
     * @return A raw pointer to the stored entity, or nullptr when the input is null.
     */
    Entity* Add(std::unique_ptr<Entity> entity);

    /**
     * @brief Mark an entity for safe deferred removal from the world.
     * @param entity The entity instance to remove.
     * @return True when the entity was found and marked.
     */
    bool Remove(Entity* entity);

    /**
     * @brief Update every live entity currently stored in the world.
     * @param dt Elapsed time in seconds since the previous update.
     */
    void Update(float dt);

    /**
     * @brief Render a transient isometric-sorted view of the world.
     * @param ctx Rendering data shared across the current frame.
     */
    void Render(const RenderContext& ctx) const;

    /**
     * @brief Remove all entities and clear any queued additions.
     */
    void Clear();

    /**
     * @brief Check whether the world still owns the supplied entity pointer.
     * @param entity Raw entity pointer to test.
     * @return True when the pointer belongs to the world or its pending additions.
     */
    bool Contains(const Entity* entity) const;

    /**
     * @brief Return the first entity that matches the supplied category.
     * @param category Category to search for.
     * @return The first matching entity, or nullptr when none match.
     */
    Entity* FindFirstByCategory(EntityCategory category);
    const Entity* FindFirstByCategory(EntityCategory category) const;

    /**
     * @brief Count the number of live entities in the supplied category.
     * @param category Category to count.
     * @return The number of matching entities.
     */
    std::size_t CountByCategory(EntityCategory category) const;

    /**
     * @brief Visit each live entity currently owned by the world.
     * @param func Callback that receives each entity by reference.
     */
    template <typename Func>
    void ForEachEntity(Func&& func)
    {
        for (const auto& entity : entities)
        {
            if (!entity->IsPendingDestroy())
            {
                func(*entity);
            }
        }
    }

    /**
     * @brief Visit each live entity currently owned by the world.
     * @param func Callback that receives each entity by const reference.
     */
    template <typename Func>
    void ForEachEntity(Func&& func) const
    {
        for (const auto& entity : entities)
        {
            if (!entity->IsPendingDestroy())
            {
                func(*entity);
            }
        }
    }

    /**
     * @brief Return the first live entity of the requested runtime type.
     * @return The first matching entity, or nullptr when none match.
     */
    template <typename T>
    T* FindFirstOfType()
    {
        static_assert(std::is_base_of_v<Entity, T>, "T must derive from Entity.");

        for (const auto& entity : entities)
        {
            if (entity->IsPendingDestroy())
            {
                continue;
            }

            if (T* typedEntity = dynamic_cast<T*>(entity.get()))
            {
                return typedEntity;
            }
        }

        return nullptr;
    }

    /**
     * @brief Return the first live entity of the requested runtime type.
     * @return The first matching entity, or nullptr when none match.
     */
    template <typename T>
    const T* FindFirstOfType() const
    {
        static_assert(std::is_base_of_v<Entity, T>, "T must derive from Entity.");

        for (const auto& entity : entities)
        {
            if (entity->IsPendingDestroy())
            {
                continue;
            }

            if (const T* typedEntity = dynamic_cast<const T*>(entity.get()))
            {
                return typedEntity;
            }
        }

        return nullptr;
    }

    /**
     * @brief Return the number of live entities currently owned by the world.
     * @return The number of stored entities excluding pending-destroy items.
     */
    std::size_t Count() const;

private:
    using EntityStorage = std::vector<std::unique_ptr<Entity>>;

    /**
     * @brief Move entities queued during update into the main storage.
     */
    void FlushPendingAdditions();

    /**
     * @brief Destroy entities that were marked for deferred removal.
     */
    void CollectPendingDestroy();

    /**
     * @brief Build a transient render view sorted for isometric drawing.
     * @return A sorted list of non-owning entity pointers.
     */
    std::vector<const Entity*> BuildRenderList() const;

    EntityStorage entities;
    EntityStorage pendingAdditions;
    bool isUpdating = false;
};
