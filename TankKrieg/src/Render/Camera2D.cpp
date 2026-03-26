#include "Render/Camera2D.h"
#include "Math/IsoUtils.h"

/**
 * @brief Store the viewport size used by the camera projection.
 * @param widthPx Viewport width in pixels.
 * @param heightPx Viewport height in pixels.
 */
void Camera2D::SetViewportSize(int widthPx, int heightPx)
{
    viewportWpx = widthPx;
    viewportHpx = heightPx;
}

/**
 * @brief Store the grid-space position that the camera should center on.
 * @param gridPosTiles Target position in tile coordinates.
 */
void Camera2D::SetTargetGrid(const Vector2& gridPosTiles)
{
    targetGridTiles = gridPosTiles;
}

/**
 * @brief Update the projection origin so the target appears at the viewport center.
 * @param tileWpx Tile width in pixels used by the isometric projection.
 * @param tileHpx Tile height in pixels used by the isometric projection.
 */
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

/**
 * @brief Return the current horizontal projection origin.
 * @return The X offset applied during world-to-screen projection.
 */
int Camera2D::OriginXpx() const
{
    return originXpx;
}

/**
 * @brief Return the current vertical projection origin.
 * @return The Y offset applied during world-to-screen projection.
 */
int Camera2D::OriginYpx() const
{
    return originYpx;
}
