#include <cmath>

#include "Entities/Tank.h"

namespace
{
    Vector2 NormalizeSafe(const Vector2& value)
    {
        const float lengthSquared = value.x * value.x + value.y * value.y;
        if (lengthSquared < 0.000001f)
        {
            return { 0.0f, 0.0f };
        }

        const float inverseLength = 1.0f / std::sqrt(lengthSquared);
        return { value.x * inverseLength, value.y * inverseLength };
    }
}

Tank::Tank()
{
    SetMoveSpeed(2.5f);
}

float Tank::AngleFromVectorVisual(const Vector2& v)
{
    return std::atan2(v.y, v.x);
}

Vector2 Tank::GetGridPosition() const
{
    return GetWorldPosition();
}

void Tank::SetGridPosition(float gxTiles, float gyTiles)
{
    SetWorldPosition(gxTiles, gyTiles);
}

Vector2 Tank::GetRenderSortPoint() const
{
    return GetWorldPosition();
}

TankVisualState Tank::BuildVisualState() const
{
    TankVisualState visualState{};
    visualState.worldPosition = GetRenderSortPoint();
    visualState.hullAngleRadians = hullAngleRad;
    visualState.turretAngleRadians = turretAngleRad;
    return visualState;
}

void Tank::Update(float)
{
    const Vector2 moveDirection = NormalizeSafe(moveVisual);
    if (moveDirection.x != 0.0f || moveDirection.y != 0.0f)
    {
        hullAngleRad = AngleFromVectorVisual(moveDirection);
    }

    const Vector2 aimDirection = NormalizeSafe(aimVisual);
    if (aimDirection.x != 0.0f || aimDirection.y != 0.0f)
    {
        turretAngleRad = AngleFromVectorVisual(aimDirection);
    }
}
