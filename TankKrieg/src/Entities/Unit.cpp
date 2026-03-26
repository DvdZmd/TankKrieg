#include "Entities/Unit.h"

void Unit::SetMovementDirection(const Vector2& direction)
{
    movementDirection = direction;
}

Vector2 Unit::GetMovementDirection() const
{
    return movementDirection;
}

void Unit::SetMoveSpeed(float speedTilesPerSecond)
{
    speedTilesPerSec = speedTilesPerSecond;
}

float Unit::GetMoveSpeed() const
{
    return speedTilesPerSec;
}

void Unit::Update(float dt)
{
    Translate(movementDirection * speedTilesPerSec * dt);
}
