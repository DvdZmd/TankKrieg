#pragma once

#include "Math/Int2.h"
#include "Math/Vector2.h"

class Tank;
struct TankMovementIntent;
class TileMap;

class TankMovementResolver
{
public:
    void ApplyMovement(Tank& tank, const TankMovementIntent& movementIntent, float deltaTime, const TileMap& tileMap) const;

private:
    static Vector2 BuildGridDirection(const TankMovementIntent& movementIntent);
    static Int2 WorldPositionToTile(const Vector2& worldPosition);
    static bool CanEnterTile(const TileMap& tileMap, const Int2& tile);
};
