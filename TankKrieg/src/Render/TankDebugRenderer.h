#pragma once

#include <SDL3/SDL.h>

#include "Render/RenderContext.h"
#include "Render/TankRenderData.h"

struct TankDebugStyle
{
    float shadowAlpha = 0.38f;
    float shadowScale = 0.72f;
    float shadowYOffsetPx = 4.0f;
    float hullForwardPx = 29.0f;
    float hullBackPx = 18.0f;
    float hullHalfWidthPx = 16.0f;
    float hullThicknessPx = 3.5f;
    float turretForwardPx = 16.0f;
    float turretBackPx = 11.0f;
    float turretHalfWidthPx = 9.5f;
    float turretThicknessPx = 3.0f;
    float barrelLengthPx = 41.0f;
    float barrelThicknessPx = 4.0f;
};

class TankDebugRenderer
{
public:
    static void Render(const TankRenderData& tankRenderData, const RenderContext& ctx, const TankDebugStyle& style = TankDebugStyle{});
};
