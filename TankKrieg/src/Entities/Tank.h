#pragma once
#include "Entities/Unit.h"

class Tank : public Unit
{
public:
    Tank() { speed = 2.5f; }

    Vector2 GetGridPosition() const { return position; }
    void Update(float dt) override;
    void SetGridPosition(float gx, float gy);
    void Render(const RenderContext& ctx) const override;
    void SetMoveVisual(const Vector2& value) { moveVisual = value; }
    void SetAimVisual(const Vector2& value) { aimVisual = value; }
    void SetMoveSpeed(float tilesPerSecond) { speed = tilesPerSecond; }// TODO not implemented yet

private:
    Vector2 moveVisual;// moveVisual: left stick (visual/screen space, normalized-ish)
    Vector2 aimVisual;// aimVisual : right stick (visual/screen space, normalized-ish)

    float hullAngleRad = 0.0f;   // in visual/screen space
    float turretAngleRad = 0.0f; // in visual/screen space

private:
    static float AngleFromVectorVisual(const Vector2& v);
};
