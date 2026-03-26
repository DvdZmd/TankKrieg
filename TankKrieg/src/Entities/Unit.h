#pragma once
#include "Entity.h"

class Unit : public Entity {
public:
    /**
     * @brief Set the current movement direction for the unit.
     * @param dir Direction in grid space used during subsequent updates.
     */
    void SetDirection(const Vector2& dir);
    /**
     * @brief Move the unit according to its direction and speed.
     * @param dt Elapsed time in seconds since the previous update.
     */
    void Update(float dt) override;

protected:
    Vector2 direction;
    float speedTilesPerSec = 150.0f;
};
