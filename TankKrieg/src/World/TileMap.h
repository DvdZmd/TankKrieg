#pragma once

class TileMap
{
public:
    /**
     * @brief Construct a tile map with fixed grid dimensions.
     * @param w Number of tiles along the X axis.
     * @param h Number of tiles along the Y axis.
     */
    TileMap(int w, int h) : width(w), height(h) {}

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

private:
    int width = 0;
    int height = 0;
};
