#pragma once

#include "Render/RenderContext.h"
#include "Entities/TankVisualState.h"

struct TankVisualDefinition;

class TankRenderer
{
public:
    void Render(const TankVisualState& tankVisualState, const RenderContext& ctx, const TankVisualDefinition* visualDefinition) const;
};
