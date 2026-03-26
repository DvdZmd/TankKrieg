#pragma once

#include <array>

#include <SDL3/SDL.h>

#include "Math/Vector2.h"

struct DirectionalSpriteSet
{
    static constexpr int kDirectionCount = 8;

    SDL_Texture* texture = nullptr;
    std::array<SDL_FRect, kDirectionCount> sourceRects{};
    Vector2 sizePx{ 0.0f, 0.0f };
    Vector2 originPx{ 0.0f, 0.0f };

    bool IsValid() const
    {
        return texture != nullptr && sizePx.x > 0.0f && sizePx.y > 0.0f;
    }
};

// Static render-side description for a tank appearance.
struct TankVisualDefinition
{
    DirectionalSpriteSet hull{};
    DirectionalSpriteSet turret{};

    bool IsComplete() const
    {
        return hull.IsValid() && turret.IsValid();
    }
};
