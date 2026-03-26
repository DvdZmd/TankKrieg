#pragma once

#include "Entities/Entity.h"

class StaticEntity : public Entity
{
public:
    explicit StaticEntity(EntityCategory category = EntityCategory::Obstacle);

    EntityCategory GetCategory() const override;
    bool BlocksMovement() const override;
    Vector2 GetRenderSortPoint() const override;

    void SetCategory(EntityCategory entityCategory);
    void SetBlocksMovement(bool shouldBlock);

private:
    EntityCategory category = EntityCategory::Obstacle;
    bool blocksMovement = false;
};
