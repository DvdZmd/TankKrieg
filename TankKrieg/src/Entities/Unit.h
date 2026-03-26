#pragma once

#include "Entities/Entity.h"

class Unit : public Entity
{
public:
    EntityCategory GetCategory() const override { return EntityCategory::Unit; }

    void SetMovementDirection(const Vector2& direction);
    Vector2 GetMovementDirection() const;

    void SetMoveSpeed(float speedTilesPerSecond);
    float GetMoveSpeed() const;

    void Update(float dt) override;

protected:
    Vector2 movementDirection{};
    float speedTilesPerSec = 1.0f;
};
