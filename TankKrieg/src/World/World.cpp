#include "World/World.h"

/**
 * @brief Add an entity to the world while preserving unique ownership.
 * @param entity Entity instance transferred into world ownership.
 * @return A raw pointer to the stored entity, or nullptr when the input is null.
 */
Entity* World::Add(std::unique_ptr<Entity> entity)
{
    if (!entity)
    {
        return nullptr;
    }

    Entity* rawEntity = entity.get();

    if (isUpdating)
    {
        pendingAdditions.push_back(std::move(entity));
    }
    else
    {
        entities.push_back(std::move(entity));
    }

    return rawEntity;
}

/**
 * @brief Mark an entity for safe deferred removal from the world.
 * @param entity The entity instance to remove.
 * @return True when the entity was found and marked.
 */
bool World::Remove(Entity* entity)
{
    if (entity == nullptr || !Contains(entity))
    {
        return false;
    }

    entity->Destroy();
    return true;
}

/**
 * @brief Update all live entities currently contained in the world.
 * @param dt Elapsed time in seconds since the previous update.
 */
void World::Update(float dt)
{
    isUpdating = true;

    for (auto& entity : entities)
    {
        if (!entity->IsPendingDestroy() && entity->IsActive())
        {
            entity->Update(dt);
        }
    }

    isUpdating = false;

    FlushPendingAdditions();
    CollectPendingDestroy();
}

/**
 * @brief Render all live entities using an isometric-friendly draw order.
 * @param ctx Rendering data shared across the current frame.
 */
void World::Render(const RenderContext& ctx) const
{
    const std::vector<const Entity*> renderList = BuildRenderList();

    for (const Entity* entity : renderList)
    {
        if (entity->IsVisible())
        {
            entity->Render(ctx);
        }
    }
}

/**
 * @brief Remove every entity owned by the world.
 */
void World::Clear()
{
    pendingAdditions.clear();
    entities.clear();
}

/**
 * @brief Check whether the world still owns the supplied entity pointer.
 * @param entity Raw entity pointer to test.
 * @return True when the pointer belongs to the world or its pending additions.
 */
bool World::Contains(const Entity* entity) const
{
    if (entity == nullptr)
    {
        return false;
    }

    const auto matchesPointer = [entity](const auto& candidate)
    {
        return candidate.get() == entity;
    };

    return std::any_of(entities.begin(), entities.end(), matchesPointer) ||
        std::any_of(pendingAdditions.begin(), pendingAdditions.end(), matchesPointer);
}

/**
 * @brief Return the first entity that matches the supplied category.
 * @param category Category to search for.
 * @return The first matching entity, or nullptr when none match.
 */
Entity* World::FindFirstByCategory(EntityCategory category)
{
    for (const auto& entity : entities)
    {
        if (!entity->IsPendingDestroy() && entity->GetCategory() == category)
        {
            return entity.get();
        }
    }

    return nullptr;
}

/**
 * @brief Return the first entity that matches the supplied category.
 * @param category Category to search for.
 * @return The first matching entity, or nullptr when none match.
 */
const Entity* World::FindFirstByCategory(EntityCategory category) const
{
    for (const auto& entity : entities)
    {
        if (!entity->IsPendingDestroy() && entity->GetCategory() == category)
        {
            return entity.get();
        }
    }

    return nullptr;
}

/**
 * @brief Count the number of live entities in the supplied category.
 * @param category Category to count.
 * @return The number of matching entities.
 */
std::size_t World::CountByCategory(EntityCategory category) const
{
    std::size_t count = 0;

    for (const auto& entity : entities)
    {
        if (!entity->IsPendingDestroy() && entity->GetCategory() == category)
        {
            ++count;
        }
    }

    return count;
}

/**
 * @brief Return the number of live entities currently owned by the world.
 * @return The number of stored entities excluding pending-destroy items.
 */
std::size_t World::Count() const
{
    std::size_t count = 0;

    for (const auto& entity : entities)
    {
        if (!entity->IsPendingDestroy())
        {
            ++count;
        }
    }

    return count;
}

/**
 * @brief Move entities queued during update into the main storage.
 */
void World::FlushPendingAdditions()
{
    if (pendingAdditions.empty())
    {
        return;
    }

    entities.reserve(entities.size() + pendingAdditions.size());

    for (auto& entity : pendingAdditions)
    {
        entities.push_back(std::move(entity));
    }

    pendingAdditions.clear();
}

/**
 * @brief Destroy entities that were marked for deferred removal.
 */
void World::CollectPendingDestroy()
{
    const auto newEnd = std::remove_if(
        entities.begin(),
        entities.end(),
        [](const std::unique_ptr<Entity>& entity)
        {
            return entity->IsPendingDestroy();
        });

    entities.erase(newEnd, entities.end());

    const auto pendingEnd = std::remove_if(
        pendingAdditions.begin(),
        pendingAdditions.end(),
        [](const std::unique_ptr<Entity>& entity)
        {
            return entity->IsPendingDestroy();
        });

    pendingAdditions.erase(pendingEnd, pendingAdditions.end());
}

/**
 * @brief Build a transient render view sorted for isometric drawing.
 * @return A sorted list of non-owning entity pointers.
 */
std::vector<const Entity*> World::BuildRenderList() const
{
    std::vector<const Entity*> renderList;
    renderList.reserve(entities.size());

    for (const auto& entity : entities)
    {
        if (!entity->IsPendingDestroy())
        {
            renderList.push_back(entity.get());
        }
    }

    std::stable_sort(
        renderList.begin(),
        renderList.end(),
        [](const Entity* lhs, const Entity* rhs)
        {
            const Vector2 leftAnchor = lhs->GetRenderSortPoint();
            const Vector2 rightAnchor = rhs->GetRenderSortPoint();

            const float leftDepth = leftAnchor.x + leftAnchor.y;
            const float rightDepth = rightAnchor.x + rightAnchor.y;

            if (leftDepth != rightDepth)
            {
                return leftDepth < rightDepth;
            }

            if (leftAnchor.y != rightAnchor.y)
            {
                return leftAnchor.y < rightAnchor.y;
            }

            return leftAnchor.x < rightAnchor.x;
        });

    return renderList;
}
