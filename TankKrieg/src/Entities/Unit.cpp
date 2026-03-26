#include "Unit.h"

/**
 * @brief Store the movement direction used by the base unit update.
 * @param dir Direction in grid space for future movement updates.
 */
void Unit::SetDirection(const Vector2& dir) {
    direction = dir;
}

/**
 * @brief Advance the unit position using its current direction and speed.
 * @param dt Elapsed time in seconds since the previous update.
 */
void Unit::Update(float dt) {
    position += direction * speedTilesPerSec * dt;
}
