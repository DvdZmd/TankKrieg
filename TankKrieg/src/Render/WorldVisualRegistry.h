#pragma once

#include <variant>
#include <vector>

class Entity;
struct TankVisualDefinition;

class WorldVisualDefinitionBinding
{
public:
    WorldVisualDefinitionBinding() = default;

    static WorldVisualDefinitionBinding Tank(const TankVisualDefinition* visualDefinition)
    {
        return WorldVisualDefinitionBinding(visualDefinition);
    }

    const TankVisualDefinition* GetTank() const
    {
        const auto* tankVisual = std::get_if<const TankVisualDefinition*>(&value);
        return tankVisual != nullptr ? *tankVisual : nullptr;
    }

private:
    using Value = std::variant<std::monostate, const TankVisualDefinition*>;

    explicit WorldVisualDefinitionBinding(const TankVisualDefinition* visualDefinition)
        : value(visualDefinition)
    {
    }

    Value value{};
};

class WorldVisualRegistry
{
public:
    void Clear();
    void Bind(const Entity* entity, const WorldVisualDefinitionBinding& visualDefinition);
    WorldVisualDefinitionBinding Resolve(const Entity& entity) const;

private:
    struct WorldVisualBinding
    {
        const Entity* entity = nullptr;
        WorldVisualDefinitionBinding visualDefinition{};
    };

    std::vector<WorldVisualBinding> bindings;
};
