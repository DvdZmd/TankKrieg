#pragma once
#include "Entities/Unit.h"

class Tank : public Unit
{
public:
    Tank() { speedTilesPerSec = 2.5f; }

    Vector2 GetGridPosition() const { return position; }
    void Update(float dt) override;
    void SetGridPosition(float gxTiles, float gyTiles);
    void Render(const RenderContext& ctx) const override;
    void SetMoveVisual(const Vector2& value) { moveVisual = value; }
    void SetAimVisual(const Vector2& value) { aimVisual = value; }
    void SetMoveSpeed(float speedTilesPerSecond) { speedTilesPerSec = speedTilesPerSecond; }

private:
    Vector2 moveVisual;
    Vector2 aimVisual;

    float hullAngleRad = 0.0f;   // in visual/screen space
    float turretAngleRad = 0.0f; // in visual/screen space

private:
    static float AngleFromVectorVisual(const Vector2& v);
};
