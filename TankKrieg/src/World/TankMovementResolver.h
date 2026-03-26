#pragma once

#include "Math/Int2.h"
#include "Math/Vector2.h"

class Tank;
class TileMap;

class TankMovementResolver
{
public:
    void ApplyMovement(Tank& tank, float deltaTime, const TileMap& tileMap) const;

private:
    static Int2 WorldPositionToTile(const Vector2& worldPosition);
    static bool CanEnterTile(const TileMap& tileMap, const Int2& tile);
};
