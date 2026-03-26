#pragma once
#include "Entities/Unit.h"

class Tank : public Unit
{
public:
    /**
     * @brief Construct a tank with its default movement speed.
     */
    Tank() { speedTilesPerSec = 2.5f; }

    /**
     * @brief Return the tank position in grid space.
     * @return The current grid-space position measured in tiles.
     */
    Vector2 GetGridPosition() const { return position; }
    /**
     * @brief Update hull movement and turret aiming for the current frame.
     * @param dt Elapsed time in seconds since the previous update.
     */
    void Update(float dt) override;
    /**
     * @brief Set the tank position directly in grid-space tile coordinates.
     * @param gxTiles Grid X position measured in tiles.
     * @param gyTiles Grid Y position measured in tiles.
     */
    void SetGridPosition(float gxTiles, float gyTiles);
    /**
     * @brief Render the tank hull, turret, and supporting debug shapes.
     * @param ctx Rendering data shared across the current frame.
     */
    void Render(const RenderContext& ctx) const override;
    /**
     * @brief Store the visual movement input that drives hull movement.
     * @param value Visual-space movement direction for the tank hull.
     */
    void SetMoveVisual(const Vector2& value) { moveVisual = value; }
    /**
     * @brief Store the visual aiming input that drives turret rotation.
     * @param value Visual-space aim direction for the turret.
     */
    void SetAimVisual(const Vector2& value) { aimVisual = value; }
    /**
     * @brief Set the tank movement speed in tiles per second.
     * @param speedTilesPerSecond Movement speed applied during hull updates.
     */
    void SetMoveSpeed(float speedTilesPerSecond) { speedTilesPerSec = speedTilesPerSecond; }

private:
    Vector2 moveVisual;
    Vector2 aimVisual;

    float hullAngleRad = 0.0f;   // in visual/screen space
    float turretAngleRad = 0.0f; // in visual/screen space

private:
    /**
     * @brief Convert a visual direction vector into an SDL-compatible angle.
     * @param v Direction in visual or screen space.
     * @return The angle in radians matching the supplied direction.
     */
    static float AngleFromVectorVisual(const Vector2& v);
};
