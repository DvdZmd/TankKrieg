#pragma once

#include "Math/Int2.h"
#include "Math/Vector2.h"

struct TankMovementIntent
{
    Vector2 visualDirection{};
    Int2 visualStep{};

    static TankMovementIntent FromVisualDirection(const Vector2& visualDirection, float deadzone = 0.25f)
    {
        TankMovementIntent intent{};
        intent.visualDirection = visualDirection;

        const float lengthSquared = visualDirection.x * visualDirection.x + visualDirection.y * visualDirection.y;
        if (lengthSquared < deadzone * deadzone)
        {
            return intent;
        }

        intent.visualStep.x = (visualDirection.x > deadzone) ? 1 : (visualDirection.x < -deadzone ? -1 : 0);
        intent.visualStep.y = (visualDirection.y > deadzone) ? 1 : (visualDirection.y < -deadzone ? -1 : 0);
        return intent;
    }

    bool HasMovement() const
    {
        return visualStep.x != 0 || visualStep.y != 0;
    }
};
