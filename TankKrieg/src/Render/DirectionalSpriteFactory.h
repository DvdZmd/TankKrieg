#pragma once

#include "Render/TankVisualDefinition.h"

class DirectionalSpriteFactory
{
public:
    static DirectionalSpriteSet BuildHorizontalStrip(SDL_Texture* texture, const Vector2& originFactor = { 0.5f, 0.8f });
};
