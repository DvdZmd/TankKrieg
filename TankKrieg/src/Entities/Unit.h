#pragma once
#include "Entity.h"

class Unit : public Entity {
public:
    void SetDirection(const Vector2& dir);
    void Update(float dt) override;

protected:
    Vector2 direction;   // hacia dónde quiere ir
    float speed = 150.0f;
};