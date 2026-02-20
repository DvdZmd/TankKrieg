#pragma once
#include "Render/RenderContext.h"
#include "Math/Int2.h"
#include "Math/Vector2.h"

class DebugOverlay
{
public:
    void SetCursorTile(const Int2& tileGrid) { cursorTileGrid = tileGrid; }
    void SetCrosshairGridTiles(const Vector2& positionGridTiles) { crosshairGridTiles = positionGridTiles; }

    void Render(const RenderContext& ctx) const;

private:
    Int2 cursorTileGrid{ 0, 0 };
    Vector2 crosshairGridTiles{ 0.0f, 0.0f };
};
