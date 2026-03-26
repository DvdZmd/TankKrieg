#include <cmath>

#include "Entities/Tank.h"

#include "Math/IsoUtils.h"

namespace
{
    Int2 SnapVisual8(const Vector2& v, float deadzone = 0.25f)
    {
        const float lenSq = v.x * v.x + v.y * v.y;
        if (lenSq < deadzone * deadzone)
        {
            return { 0, 0 };
        }

        Int2 snapped{ 0, 0 };
        snapped.x = (v.x > deadzone) ? 1 : (v.x < -deadzone ? -1 : 0);
        snapped.y = (v.y > deadzone) ? 1 : (v.y < -deadzone ? -1 : 0);
        return snapped;
    }

    Vector2 NormalizeStep(const Int2& step)
    {
        Vector2 direction{ static_cast<float>(step.x), static_cast<float>(step.y) };
        const float lengthSquared = direction.x * direction.x + direction.y * direction.y;
        if (lengthSquared < 0.000001f)
        {
            return { 0.0f, 0.0f };
        }

        const float inverseLength = 1.0f / SDL_sqrtf(lengthSquared);
        return { direction.x * inverseLength, direction.y * inverseLength };
    }

    float LengthSq(const Vector2& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    Vector2 NormalizeSafe(const Vector2& v)
    {
        const float lengthSquared = LengthSq(v);
        if (lengthSquared < 0.000001f)
        {
            return { 0.0f, 0.0f };
        }

        const float inverseLength = 1.0f / SDL_sqrtf(lengthSquared);
        return { v.x * inverseLength, v.y * inverseLength };
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

TankRenderData Tank::BuildRenderData() const
{
    TankRenderData renderData{};
    renderData.worldPosition = GetRenderSortPoint();
    renderData.hullAngleRadians = hullAngleRad;
    renderData.turretAngleRadians = turretAngleRad;
    return renderData;
}

Vector2 Tank::ComputeMoveDelta(float dt) const
{
    const Int2 moveSnap = SnapVisual8(moveVisual);
    if (moveSnap.x == 0 && moveSnap.y == 0)
    {
        return { 0.0f, 0.0f };
    }

    const Int2 gridStep = IsoUtils::VisualToIsoGridStep(moveSnap);
    const Vector2 gridDirection = NormalizeStep(gridStep);
    return gridDirection * GetMoveSpeed() * dt;
}

void Tank::Update(float)
{
    const Int2 moveSnap = SnapVisual8(moveVisual);
    if (moveSnap.x != 0 || moveSnap.y != 0)
    {
        const Vector2 visualDirection = NormalizeStep(moveSnap);
        hullAngleRad = AngleFromVectorVisual(visualDirection);
    }

    const float aimLengthSquared = aimVisual.x * aimVisual.x + aimVisual.y * aimVisual.y;
    if (aimLengthSquared > 0.0004f)
    {
        turretAngleRad = AngleFromVectorVisual(NormalizeSafe(aimVisual));
    }
}
