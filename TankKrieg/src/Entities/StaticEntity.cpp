#include "Entities/StaticEntity.h"

StaticEntity::StaticEntity(EntityCategory entityCategory)
    : category(entityCategory)
{
}

EntityCategory StaticEntity::GetCategory() const
{
    return category;
}

bool StaticEntity::BlocksMovement() const
{
    return blocksMovement;
}

void StaticEntity::SetCategory(EntityCategory entityCategory)
{
    category = entityCategory;
}

void StaticEntity::SetBlocksMovement(bool shouldBlock)
{
    blocksMovement = shouldBlock;
}
