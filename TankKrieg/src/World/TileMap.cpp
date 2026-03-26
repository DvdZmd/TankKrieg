#include "World/TileMap.h"

TileMap::TileMap(int w, int h)
    : width(w)
    , height(h)
    , tiles(static_cast<std::size_t>(w * h))
{
}

bool TileMap::IsInBounds(const Int2& tile) const
{
    return tile.x >= 0 && tile.y >= 0 && tile.x < width && tile.y < height;
}

int TileMap::GetIndex(const Int2& tile) const
{
    if (!IsInBounds(tile))
    {
        return -1;
    }

    return tile.y * width + tile.x;
}

const TileData* TileMap::TryGetTile(const Int2& tile) const
{
    const int index = GetIndex(tile);
    if (index < 0)
    {
        return nullptr;
    }

    return &tiles[static_cast<std::size_t>(index)];
}

TileData* TileMap::TryGetTile(const Int2& tile)
{
    const int index = GetIndex(tile);
    if (index < 0)
    {
        return nullptr;
    }

    return &tiles[static_cast<std::size_t>(index)];
}

bool TileMap::BlocksMovement(const Int2& tile) const
{
    const TileData* tileData = TryGetTile(tile);
    return tileData == nullptr || tileData->blocksMovement;
}

bool TileMap::SetBlocksMovement(const Int2& tile, bool value)
{
    TileData* tileData = TryGetTile(tile);
    if (tileData == nullptr)
    {
        return false;
    }

    tileData->blocksMovement = value;
    return true;
}
