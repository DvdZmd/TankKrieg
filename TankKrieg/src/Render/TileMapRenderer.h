#pragma once
#include "Render/RenderContext.h"
#include "World/TileMap.h"

class TileMapRenderer
{
public:
    /**
     * @brief Render the supplied tile map using the current frame context.
     * @param ctx Rendering data shared across the current frame.
     * @param map Tile map that defines the visible grid bounds.
     */
    void Render(const RenderContext& ctx, const TileMap& map) const;
};
