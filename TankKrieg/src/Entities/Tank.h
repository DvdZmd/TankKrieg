#pragma once
#include <SDL3/SDL.h>
#include "Math/Vector2.h"

struct GridBounds
{
    float minX = 0.0f;
    float minY = 0.0f;
    float maxX = 0.0f; // inclusive (width - 1)
    float maxY = 0.0f; // inclusive (height - 1)
};

class Tank
{
public:
    void SetGridPosition(float gx, float gy);
    Vector2 GetGridPosition() const { return { gridX, gridY }; }

    // moveVisual: left stick (visual/screen space, normalized-ish)
    // aimVisual : right stick (visual/screen space, normalized-ish)
    void Update(float dt, const Vector2& moveVisual, const Vector2& aimVisual, const GridBounds& bounds);

    void Render(SDL_Renderer* renderer, int tileW, int tileH, int originX, int originY) const;

    void SetMoveSpeed(float tilesPerSecond) { moveSpeedTilesPerSec = tilesPerSecond; }

private:
    float gridX = 0.0f;
    float gridY = 0.0f;

    float hullAngleRad = 0.0f;   // in visual/screen space
    float turretAngleRad = 0.0f; // in visual/screen space

    float moveSpeedTilesPerSec = 2.5f;

private:
    static float AngleFromVectorVisual(const Vector2& v);
};
