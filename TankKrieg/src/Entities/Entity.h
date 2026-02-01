#pragma once
#include "Math/Vector2.h"

class Entity {
public:
    virtual ~Entity() = default;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    Vector2 position;
};
