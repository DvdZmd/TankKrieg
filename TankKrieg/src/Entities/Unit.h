#pragma once

#include "Entities/Entity.h"

class Unit : public Entity
{
public:
    EntityCategory GetCategory() const override { return EntityCategory::Unit; }

    void SetMoveSpeed(float speedTilesPerSecond);
    float GetMoveSpeed() const;

protected:
    float speedTilesPerSec = 1.0f;
};
