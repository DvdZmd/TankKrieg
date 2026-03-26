#include "Entities/Unit.h"

void Unit::SetMoveSpeed(float speedTilesPerSecond)
{
    speedTilesPerSec = speedTilesPerSecond;
}

float Unit::GetMoveSpeed() const
{
    return speedTilesPerSec;
}
