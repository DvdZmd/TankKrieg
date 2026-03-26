#pragma once

#include "Math/Vector2.h"

// Dynamic gameplay-exported data consumed by renderers.
struct TankVisualState
{
    Vector2 worldPosition{};
    float hullAngleRadians = 0.0f;
    float turretAngleRadians = 0.0f;
};
