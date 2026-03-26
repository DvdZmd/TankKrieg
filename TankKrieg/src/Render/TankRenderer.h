#pragma once

#include "Render/RenderContext.h"
#include "Render/TankRenderData.h"

struct TankVisual;

class TankRenderer
{
public:
    void Render(const TankRenderData& tankRenderData, const RenderContext& ctx, const TankVisual* visual) const;
};
