#pragma once

#include "Math/Vector2.h"

struct TankVisualState
{
    Vector2 worldPosition{};
    float hullAngleRadians = 0.0f;
    float turretAngleRadians = 0.0f;
};
