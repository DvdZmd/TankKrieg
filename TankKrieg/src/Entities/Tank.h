#pragma once

#include "Entities/Unit.h"

struct TankVisual;

class Tank : public Unit
{
public:
    Tank();

    EntityCategory GetCategory() const override { return EntityCategory::Vehicle; }

    Vector2 GetGridPosition() const;
    void SetGridPosition(float gxTiles, float gyTiles);

    Vector2 GetRenderSortPoint() const override;
    bool BlocksMovement() const override { return true; }

    Vector2 ComputeMoveDelta(float dt) const;
    void Update(float dt) override;
    void Render(const RenderContext& ctx) const override;

    void SetMoveVisual(const Vector2& value) { moveVisual = value; }
    void SetAimVisual(const Vector2& value) { aimVisual = value; }
    void SetVisual(const TankVisual* value) { visual = value; }

    float GetHullAngleRadians() const { return hullAngleRad; }
    float GetTurretAngleRadians() const { return turretAngleRad; }
    const TankVisual* GetVisual() const { return visual; }

private:
    static float AngleFromVectorVisual(const Vector2& v);

    Vector2 moveVisual{};
    Vector2 aimVisual{};
    float hullAngleRad = 0.0f;
    float turretAngleRad = 0.0f;
    const TankVisual* visual = nullptr;
};
