#include <cmath>

#include "Render/TankDebugRenderer.h"

#include "Entities/Tank.h"
#include "Math/IsoUtils.h"
#include "Render/Helper.h"
#include "Render/IsoDebugDraw.h"

namespace
{
    Vector2 Rotate(const Vector2& value, float radians)
    {
        const float cosAngle = std::cos(radians);
        const float sinAngle = std::sin(radians);
        return { value.x * cosAngle - value.y * sinAngle, value.x * sinAngle + value.y * cosAngle };
    }
}

void TankDebugRenderer::Render(const Tank& tank, const RenderContext& ctx, const TankDebugStyle& style)
{
    SDL_Renderer* renderer = ctx.renderer;
    const Vector2 worldPosition = tank.GetWorldPosition();
    const SDL_FPoint centerPx = IsoUtils::GridToScreenF(
        worldPosition.x,
        worldPosition.y,
        ctx.tileWidthPx,
        ctx.tileHeightPx,
        ctx.originXPx,
        ctx.originYPx);

    const SDL_FColor shadowColor{ 0.0f, 0.0f, 0.0f, style.shadowAlpha };
    IsoDebugDraw::FillIsoDiamond(
        renderer,
        centerPx.x,
        centerPx.y + style.shadowYOffsetPx,
        ctx.tileWidthPx,
        ctx.tileHeightPx,
        shadowColor,
        style.shadowScale);

    const Vector2 hullForward = Rotate({ 1.0f, 0.0f }, tank.GetHullAngleRadians());
    const Vector2 hullRight = Rotate({ 0.0f, 1.0f }, tank.GetHullAngleRadians());

    const SDL_FPoint hullA{
        centerPx.x + hullForward.x * style.hullForwardPx,
        centerPx.y + hullForward.y * style.hullForwardPx
    };
    const SDL_FPoint hullB{
        centerPx.x - hullForward.x * style.hullBackPx + hullRight.x * style.hullHalfWidthPx,
        centerPx.y - hullForward.y * style.hullBackPx + hullRight.y * style.hullHalfWidthPx
    };
    const SDL_FPoint hullC{
        centerPx.x - hullForward.x * style.hullBackPx - hullRight.x * style.hullHalfWidthPx,
        centerPx.y - hullForward.y * style.hullBackPx - hullRight.y * style.hullHalfWidthPx
    };

    RenderHelper::DrawTriangleThick(
        renderer,
        hullA,
        hullB,
        hullC,
        style.hullThicknessPx,
        SDL_FColor{ 80.0f / 255.0f, 200.0f / 255.0f, 120.0f / 255.0f, 1.0f });

    const Vector2 turretForward = Rotate({ 1.0f, 0.0f }, tank.GetTurretAngleRadians());
    const Vector2 turretRight = Rotate({ 0.0f, 1.0f }, tank.GetTurretAngleRadians());

    const SDL_FPoint turretA{
        centerPx.x + turretForward.x * style.turretForwardPx,
        centerPx.y + turretForward.y * style.turretForwardPx
    };
    const SDL_FPoint turretB{
        centerPx.x - turretForward.x * style.turretBackPx + turretRight.x * style.turretHalfWidthPx,
        centerPx.y - turretForward.y * style.turretBackPx + turretRight.y * style.turretHalfWidthPx
    };
    const SDL_FPoint turretC{
        centerPx.x - turretForward.x * style.turretBackPx - turretRight.x * style.turretHalfWidthPx,
        centerPx.y - turretForward.y * style.turretBackPx - turretRight.y * style.turretHalfWidthPx
    };

    RenderHelper::DrawTriangleThick(
        renderer,
        turretA,
        turretB,
        turretC,
        style.turretThicknessPx,
        SDL_FColor{ 40.0f / 255.0f, 140.0f / 255.0f, 220.0f / 255.0f, 1.0f });

    const SDL_FPoint muzzle{
        centerPx.x + turretForward.x * style.barrelLengthPx,
        centerPx.y + turretForward.y * style.barrelLengthPx
    };
    RenderHelper::DrawThickLine(
        renderer,
        centerPx,
        muzzle,
        style.barrelThicknessPx,
        SDL_FColor{ 40.0f / 255.0f, 140.0f / 255.0f, 220.0f / 255.0f, 1.0f });
}
