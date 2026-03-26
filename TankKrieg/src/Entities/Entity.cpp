#include "Entities/Entity.h"

void Entity::Update(float)
{
}

EntityCategory Entity::GetCategory() const
{
    return EntityCategory::Unknown;
}

Vector2 Entity::GetRenderSortPoint() const
{
    return worldPosition;
}

bool Entity::BlocksMovement() const
{
    return false;
}

Vector2 Entity::GetWorldPosition() const
{
    return worldPosition;
}

void Entity::SetWorldPosition(const Vector2& position)
{
    worldPosition = position;
}

void Entity::SetWorldPosition(float x, float y)
{
    worldPosition = { x, y };
}

void Entity::Translate(const Vector2& delta)
{
    worldPosition += delta;
}

bool Entity::IsActive() const
{
    return active;
}

void Entity::SetActive(bool value)
{
    active = value;
}

bool Entity::IsVisible() const
{
    return visible;
}

void Entity::SetVisible(bool value)
{
    visible = value;
}

void Entity::Destroy()
{
    pendingDestroy = true;
}

void Entity::MarkForDestroy()
{
    Destroy();
}

bool Entity::IsPendingDestroy() const
{
    return pendingDestroy;
}
