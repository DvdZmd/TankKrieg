#include <cmath>

#include "World/TankMovementResolver.h"

#include "Entities/Tank.h"
#include "Entities/TankMovementIntent.h"
#include "Math/IsoUtils.h"
#include "World/TileMap.h"

namespace
{
    Vector2 NormalizeStep(const Int2& step)
    {
        Vector2 direction{ static_cast<float>(step.x), static_cast<float>(step.y) };
        const float lengthSquared = direction.x * direction.x + direction.y * direction.y;
        if (lengthSquared < 0.000001f)
        {
            return { 0.0f, 0.0f };
        }

        const float inverseLength = 1.0f / std::sqrt(lengthSquared);
        return { direction.x * inverseLength, direction.y * inverseLength };
    }
}

void TankMovementResolver::ApplyMovement(Tank& tank, const TankMovementIntent& movementIntent, float deltaTime, const TileMap& tileMap) const
{
    const Vector2 gridDirection = BuildGridDirection(movementIntent);
    if (gridDirection.x == 0.0f && gridDirection.y == 0.0f)
    {
        return;
    }

    const Vector2 moveDelta = gridDirection * tank.GetMoveSpeed() * deltaTime;
    if (moveDelta.x == 0.0f && moveDelta.y == 0.0f)
    {
        return;
    }

    const Vector2 currentPosition = tank.GetWorldPosition();
    const Vector2 nextPosition = currentPosition + moveDelta;
    if (CanEnterTile(tileMap, WorldPositionToTile(nextPosition)))
    {
        tank.SetWorldPosition(nextPosition);
        return;
    }

    const Vector2 xOnlyPosition{ currentPosition.x + moveDelta.x, currentPosition.y };
    if (CanEnterTile(tileMap, WorldPositionToTile(xOnlyPosition)))
    {
        tank.SetWorldPosition(xOnlyPosition);
    }

    const Vector2 yOnlyPosition{ tank.GetWorldPosition().x, currentPosition.y + moveDelta.y };
    if (CanEnterTile(tileMap, WorldPositionToTile(yOnlyPosition)))
    {
        tank.SetWorldPosition(yOnlyPosition);
    }
}

Vector2 TankMovementResolver::BuildGridDirection(const TankMovementIntent& movementIntent)
{
    if (!movementIntent.HasMovement())
    {
        return { 0.0f, 0.0f };
    }

    const Int2 gridStep = IsoUtils::VisualToIsoGridStep(movementIntent.visualStep);
    return NormalizeStep(gridStep);
}

Int2 TankMovementResolver::WorldPositionToTile(const Vector2& worldPosition)
{
    return Int2{
        static_cast<int>(std::floor(worldPosition.x + 0.5f)),
        static_cast<int>(std::floor(worldPosition.y + 0.5f))
    };
}

bool TankMovementResolver::CanEnterTile(const TileMap& tileMap, const Int2& tile)
{
    return !tileMap.BlocksMovement(tile);
}
