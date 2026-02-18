#pragma once
#include "Math/Vector2.h"
#include "Render/RenderContext.h"

class Entity {
public:
    virtual ~Entity() = default;

    virtual void Update(float dt) = 0;
    virtual void Render(const RenderContext& ctx) const = 0;

    Vector2 position;
};
