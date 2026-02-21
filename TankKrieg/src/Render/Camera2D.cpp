#include "Render/Camera2D.h"
#include "Math/IsoUtils.h"

void Camera2D::SetViewportSize(int widthPx, int heightPx)
{
    viewportWpx = widthPx;
    viewportHpx = heightPx;
}

void Camera2D::SetTargetGrid(const Vector2& gridPosTiles)
{
    targetGridTiles = gridPosTiles;
}

void Camera2D::Update(int tileWpx, int tileHpx)
{
    const SDL_FPoint targetScreen = IsoUtils::GridToScreenF(
        targetGridTiles.x,
        targetGridTiles.y,
        tileWpx,
        tileHpx,
        0,
        0
    );

    originXpx = (viewportWpx / 2) - static_cast<int>(targetScreen.x);
    originYpx = (viewportHpx / 2) - static_cast<int>(targetScreen.y);
}

int Camera2D::OriginXpx() const
{
    return originXpx;
}

int Camera2D::OriginYpx() const
{
    return originYpx;
}
