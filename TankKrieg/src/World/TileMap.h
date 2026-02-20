#pragma once

class TileMap
{
public:
    TileMap(int w, int h) : width(w), height(h) {}

    int Width() const { return width; }
    int Height() const { return height; }

private:
    int width = 0;
    int height = 0;
};
