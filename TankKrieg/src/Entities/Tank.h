#pragma once
#include "Entities/Unit.h"

class Tank : public Unit
{
public:
    Tank() { speed = 2.5f; }

    void SetGridPosition(float gx, float gy);
    Vector2 GetGridPosition() const { return position; }

    // moveVisual: left stick (visual/screen space, normalized-ish)
    // aimVisual : right stick (visual/screen space, normalized-ish)
    void Update(float dt) override;

    void Render(const RenderContext& ctx) const override;

    void SetMoveVisual(const Vector2& value) { moveVisual = value; }
    void SetAimVisual(const Vector2& value) { aimVisual = value; }

    void SetMoveSpeed(float tilesPerSecond) { speed = tilesPerSecond; }

private:
    Vector2 moveVisual;
    Vector2 aimVisual;

    float hullAngleRad = 0.0f;   // in visual/screen space
    float turretAngleRad = 0.0f; // in visual/screen space

private:
    static float AngleFromVectorVisual(const Vector2& v);
};
