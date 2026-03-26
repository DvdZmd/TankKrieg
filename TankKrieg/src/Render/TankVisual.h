#pragma once

#include <array>
#include <cmath>

#include <SDL3/SDL.h>

#include "Math/IsoUtils.h"
#include "Math/Vector2.h"
#include "Render/RenderContext.h"

struct DirectionalSpriteSet
{
    SDL_Texture* texture = nullptr;
    std::array<SDL_FRect, 8> sourceRects{};
    Vector2 sizePx{ 0.0f, 0.0f };
    Vector2 originPx{ 0.0f, 0.0f };

    bool IsValid() const
    {
        return texture != nullptr && sizePx.x > 0.0f && sizePx.y > 0.0f;
    }
};

struct TankVisual
{
    static constexpr int kDirectionCount = 8;

    DirectionalSpriteSet hull{};
    DirectionalSpriteSet turret{};

    static bool IsComplete(const TankVisual* visual)
    {
        return visual != nullptr && visual->hull.IsValid() && visual->turret.IsValid();
    }

    static int AngleToDirectionIndex(float radians)
    {
        constexpr float kTau = 6.28318530718f;
        constexpr float kSectorSize = kTau / static_cast<float>(kDirectionCount);

        float wrapped = std::fmod(radians, kTau);
        if (wrapped < 0.0f)
        {
            wrapped += kTau;
        }

        const float shifted = wrapped + (kSectorSize * 0.5f);
        return static_cast<int>(std::floor(shifted / kSectorSize)) % kDirectionCount;
    }

    static void RenderLayer(const DirectionalSpriteSet& layer, const Vector2& worldAnchor, float radians, const RenderContext& ctx)
    {
        if (!layer.IsValid())
        {
            return;
        }

        const SDL_FPoint screenAnchor = IsoUtils::GridToScreenF(
            worldAnchor.x,
            worldAnchor.y,
            ctx.tileWidthPx,
            ctx.tileHeightPx,
            ctx.originXPx,
            ctx.originYPx);

        const int directionIndex = AngleToDirectionIndex(radians);
        const SDL_FRect destinationRect{
            screenAnchor.x - layer.originPx.x,
            screenAnchor.y - layer.originPx.y,
            layer.sizePx.x,
            layer.sizePx.y
        };
        const SDL_FRect& sourceRect = layer.sourceRects[directionIndex];

        SDL_RenderTexture(ctx.renderer, layer.texture, &sourceRect, &destinationRect);
    }
};
