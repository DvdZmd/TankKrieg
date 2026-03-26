#pragma once

#include <vector>

#include "Math/Int2.h"

struct TileData
{
    bool blocksMovement = false;
};

class TileMap
{
public:
    /**
     * @brief Construct a tile map with fixed grid dimensions.
     * @param w Number of tiles along the X axis.
     * @param h Number of tiles along the Y axis.
     */
    TileMap(int w, int h);

    /**
     * @brief Return the tile map width.
     * @return The number of tiles along the X axis.
     */
    int Width() const { return width; }
    /**
     * @brief Return the tile map height.
     * @return The number of tiles along the Y axis.
     */
    int Height() const { return height; }

    bool IsInBounds(const Int2& tile) const;
    int GetIndex(const Int2& tile) const;

    const TileData* TryGetTile(const Int2& tile) const;
    TileData* TryGetTile(const Int2& tile);

    bool BlocksMovement(const Int2& tile) const;
    bool SetBlocksMovement(const Int2& tile, bool value);

private:
    int width = 0;
    int height = 0;
    std::vector<TileData> tiles;
};
