#pragma once

#include "Math/Vector2.h"

class Camera2D
{
public:
    void SetViewportSize(int widthPx, int heightPx);
    void SetTargetGrid(const Vector2& gridPosTiles);
    void Update(int tileWpx, int tileHpx);

    int OriginXpx() const;
    int OriginYpx() const;

private:
    Vector2 targetGridTiles{ 0.0f, 0.0f };
    int viewportWpx = 0;
    int viewportHpx = 0;
    int originXpx = 0;
    int originYpx = 0;
};
