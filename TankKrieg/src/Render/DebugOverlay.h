#pragma once
#include "Render/RenderContext.h"
#include "Math/Int2.h"
#include "Math/Vector2.h"

class DebugOverlay
{
public:
    /**
     * @brief Set the highlighted cursor tile shown by the debug overlay.
     * @param tileGrid Grid coordinate of the cursor tile.
     */
    void SetCursorTile(const Int2& tileGrid) { cursorTileGrid = tileGrid; }
    /**
     * @brief Set the crosshair position shown by the debug overlay.
     * @param positionGridTiles Crosshair position in grid-space tile coordinates.
     */
    void SetCrosshairGridTiles(const Vector2& positionGridTiles) { crosshairGridTiles = positionGridTiles; }

    /**
     * @brief Render cursor and crosshair debug visuals.
     * @param ctx Rendering data shared across the current frame.
     */
    void Render(const RenderContext& ctx) const;

private:
    Int2 cursorTileGrid{ 0, 0 };
    Vector2 crosshairGridTiles{ 0.0f, 0.0f };
};
