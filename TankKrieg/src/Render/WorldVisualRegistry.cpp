#include "Render/WorldVisualRegistry.h"

#include "Entities/Tank.h"
#include "Render/TankVisualDefinition.h"

void WorldVisualRegistry::Clear()
{
    tankBindings.clear();
}

void WorldVisualRegistry::BindTank(const Tank* tank, const TankVisualDefinition* visualDefinition)
{
    if (tank == nullptr)
    {
        return;
    }

    for (TankVisualBinding& binding : tankBindings)
    {
        if (binding.tank == tank)
        {
            binding.visualDefinition = visualDefinition;
            return;
        }
    }

    tankBindings.push_back(TankVisualBinding{ tank, visualDefinition });
}

const TankVisualDefinition* WorldVisualRegistry::ResolveTankVisual(const Tank& tank) const
{
    for (const TankVisualBinding& binding : tankBindings)
    {
        if (binding.tank == &tank)
        {
            return binding.visualDefinition;
        }
    }

    return nullptr;
}
