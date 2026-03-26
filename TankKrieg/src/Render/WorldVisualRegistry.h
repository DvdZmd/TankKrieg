#pragma once

#include <vector>

class Tank;
struct TankVisualDefinition;

class WorldVisualRegistry
{
public:
    void Clear();
    void BindTank(const Tank* tank, const TankVisualDefinition* visualDefinition);
    const TankVisualDefinition* ResolveTankVisual(const Tank& tank) const;

private:
    struct TankVisualBinding
    {
        const Tank* tank = nullptr;
        const TankVisualDefinition* visualDefinition = nullptr;
    };

    std::vector<TankVisualBinding> tankBindings;
};
