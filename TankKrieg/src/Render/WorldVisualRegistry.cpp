#include "Render/WorldVisualRegistry.h"

#include "Entities/Entity.h"

void WorldVisualRegistry::Clear()
{
    bindings.clear();
}

void WorldVisualRegistry::Bind(const Entity* entity, const WorldVisualDefinitionBinding& visualDefinition)
{
    if (entity == nullptr)
    {
        return;
    }

    for (WorldVisualBinding& binding : bindings)
    {
        if (binding.entity == entity)
        {
            binding.visualDefinition = visualDefinition;
            return;
        }
    }

    bindings.push_back(WorldVisualBinding{ entity, visualDefinition });
}

WorldVisualDefinitionBinding WorldVisualRegistry::Resolve(const Entity& entity) const
{
    for (const WorldVisualBinding& binding : bindings)
    {
        if (binding.entity == &entity)
        {
            return binding.visualDefinition;
        }
    }

    return {};
}
