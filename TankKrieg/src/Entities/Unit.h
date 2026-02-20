#pragma once
#include "Entity.h"

class Unit : public Entity {
public:
    void SetDirection(const Vector2& dir);
    void Update(float dt) override;

protected:
    Vector2 direction;
    float speedTilesPerSec = 150.0f;
};
