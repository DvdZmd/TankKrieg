#pragma once
#include "Render/RenderContext.h"
#include "World/TileMap.h"

class TileMapRenderer
{
public:
    void Render(const RenderContext& ctx, const TileMap& map) const;
};
