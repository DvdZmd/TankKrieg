#pragma once
#include "Math/Vector2.h"
#include "Render/RenderContext.h"

class Entity {
public:
    /**
     * @brief Destroy the entity through a polymorphic base pointer.
     */
    virtual ~Entity() = default;

    /**
     * @brief Advance the entity simulation by one frame.
     * @param dt Elapsed time in seconds since the previous update.
     */
    virtual void Update(float dt) = 0;
    /**
     * @brief Render the entity using the supplied frame context.
     * @param ctx Rendering data shared across the current frame.
     */
    virtual void Render(const RenderContext& ctx) const = 0;

    // Position in isometric grid space, measured in tiles.
    Vector2 position;
};
