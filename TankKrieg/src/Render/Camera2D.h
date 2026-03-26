#pragma once

#include "Math/Vector2.h"

class Camera2D
{
public:
    /**
     * @brief Set the viewport size used to center the camera target.
     * @param widthPx Viewport width in pixels.
     * @param heightPx Viewport height in pixels.
     */
    void SetViewportSize(int widthPx, int heightPx);
    /**
     * @brief Set the grid-space position the camera should follow.
     * @param gridPosTiles Target position in tile coordinates.
     */
    void SetTargetGrid(const Vector2& gridPosTiles);
    /**
     * @brief Recalculate the screen-space origin for the current target.
     * @param tileWpx Tile width in pixels used by the isometric projection.
     * @param tileHpx Tile height in pixels used by the isometric projection.
     */
    void Update(int tileWpx, int tileHpx);

    /**
     * @brief Return the current horizontal screen-space origin.
     * @return The X origin applied to projected world positions.
     */
    int OriginXpx() const;
    /**
     * @brief Return the current vertical screen-space origin.
     * @return The Y origin applied to projected world positions.
     */
    int OriginYpx() const;

private:
    Vector2 targetGridTiles{ 0.0f, 0.0f };
    int viewportWpx = 0;
    int viewportHpx = 0;
    int originXpx = 0;
    int originYpx = 0;
};
