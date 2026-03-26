#pragma once

#include "Math/Vector2.h"
#include "Render/RenderContext.h"

enum class EntityCategory
{
    Unknown,
    Unit,
    Vehicle,
    Obstacle,
    Decoration,
    Projectile
};

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void Update(float dt);
    virtual void Render(const RenderContext& ctx) const = 0;

    virtual EntityCategory GetCategory() const;
    /**
     * @brief Return the ground-contact anchor used for isometric sorting and sprite placement.
     * @return World-space tile position of the entity base point.
     */
    virtual Vector2 GetRenderSortPoint() const;
    virtual bool BlocksMovement() const;

    Vector2 GetWorldPosition() const;
    void SetWorldPosition(const Vector2& worldPosition);
    void SetWorldPosition(float x, float y);
    void Translate(const Vector2& delta);

    bool IsActive() const;
    void SetActive(bool active);

    bool IsVisible() const;
    void SetVisible(bool visible);

    void Destroy();
    void MarkForDestroy();
    bool IsPendingDestroy() const;

protected:
    Entity() = default;

private:
    Vector2 worldPosition{};
    bool active = true;
    bool visible = true;
    bool pendingDestroy = false;
};
