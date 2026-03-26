#pragma once

#include "Entities/Unit.h"
#include "Render/TankRenderData.h"

class Tank : public Unit
{
public:
    Tank();

    EntityCategory GetCategory() const override { return EntityCategory::Vehicle; }

    Vector2 GetGridPosition() const;
    void SetGridPosition(float gxTiles, float gyTiles);

    Vector2 GetRenderSortPoint() const override;
    bool BlocksMovement() const override { return true; }

    TankRenderData BuildRenderData() const;
    Vector2 ComputeMoveDelta(float dt) const;
    void Update(float dt) override;

    void SetMoveVisual(const Vector2& value) { moveVisual = value; }
    void SetAimVisual(const Vector2& value) { aimVisual = value; }

    float GetHullAngleRadians() const { return hullAngleRad; }
    float GetTurretAngleRadians() const { return turretAngleRad; }

private:
    static float AngleFromVectorVisual(const Vector2& v);

    Vector2 moveVisual{};
    Vector2 aimVisual{};
    float hullAngleRad = 0.0f;
    float turretAngleRad = 0.0f;
};
