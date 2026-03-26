#include <cmath>

#include "Entities/Tank.h"

#include "Math/IsoUtils.h"
#include "Render/Helper.h"
#include "Render/IsoDebugDraw.h"

static Int2 SnapVisual8(const Vector2& v, float deadzone = 0.25f)
{
    const float lenSq = v.x * v.x + v.y * v.y;
    if (lenSq < deadzone * deadzone)
    {
        return { 0, 0 };
    }

    Int2 snapped{ 0, 0 };
    snapped.x = (v.x > deadzone) ? 1 : (v.x < -deadzone ? -1 : 0);
    snapped.y = (v.y > deadzone) ? 1 : (v.y < -deadzone ? -1 : 0);
    return snapped;
}

static Vector2 NormalizeStep(const Int2& step)
{
    Vector2 direction{ static_cast<float>(step.x), static_cast<float>(step.y) };
    const float lengthSquared = direction.x * direction.x + direction.y * direction.y;
    if (lengthSquared < 0.000001f)
    {
        return { 0.0f, 0.0f };
    }

    const float inverseLength = 1.0f / SDL_sqrtf(lengthSquared);
    return { direction.x * inverseLength, direction.y * inverseLength };
}

static float LengthSq(const Vector2& v)
{
    return v.x * v.x + v.y * v.y;
}

static Vector2 NormalizeSafe(const Vector2& v)
{
    const float lengthSquared = LengthSq(v);
    if (lengthSquared < 0.000001f)
    {
        return { 0.0f, 0.0f };
    }

    const float inverseLength = 1.0f / SDL_sqrtf(lengthSquared);
    return { v.x * inverseLength, v.y * inverseLength };
}

static Vector2 Rotate(const Vector2& v, float radians)
{
    const float cosAngle = std::cos(radians);
    const float sinAngle = std::sin(radians);
    return { v.x * cosAngle - v.y * sinAngle, v.x * sinAngle + v.y * cosAngle };
}

Tank::Tank()
{
    SetMoveSpeed(2.5f);
}

float Tank::AngleFromVectorVisual(const Vector2& v)
{
    return std::atan2(v.y, v.x);
}

Vector2 Tank::GetGridPosition() const
{
    return GetWorldPosition();
}

void Tank::SetGridPosition(float gxTiles, float gyTiles)
{
    SetWorldPosition(gxTiles, gyTiles);
}

Vector2 Tank::GetRenderSortPoint() const
{
    return GetWorldPosition();
}

void Tank::Update(float dt)
{
    const Int2 moveSnap = SnapVisual8(moveVisual);
    if (moveSnap.x != 0 || moveSnap.y != 0)
    {
        const Vector2 visualDirection = NormalizeStep(moveSnap);
        hullAngleRad = AngleFromVectorVisual(visualDirection);

        const Int2 gridStep = IsoUtils::VisualToIsoGridStep(moveSnap);
        const Vector2 gridDirection = NormalizeStep(gridStep);
        Translate(gridDirection * GetMoveSpeed() * dt);
    }

    const float aimLengthSquared = aimVisual.x * aimVisual.x + aimVisual.y * aimVisual.y;
    if (aimLengthSquared > 0.0004f)
    {
        turretAngleRad = AngleFromVectorVisual(NormalizeSafe(aimVisual));
    }
}

void Tank::Render(const RenderContext& ctx) const
{
    SDL_Renderer* renderer = ctx.renderer;
    const Vector2 worldPosition = GetWorldPosition();
    const SDL_FPoint centerPx = IsoUtils::GridToScreenF(
        worldPosition.x,
        worldPosition.y,
        ctx.tileWidthPx,
        ctx.tileHeightPx,
        ctx.originXPx,
        ctx.originYPx);

    const float shadowYOffsetPx = ctx.tileHeightPx * 0.12f;
    const SDL_FColor shadowColor{ 0.0f, 0.0f, 0.0f, 0.35f };
    IsoDebugDraw::FillIsoDiamond(
        renderer,
        centerPx.x,
        centerPx.y + shadowYOffsetPx,
        ctx.tileWidthPx,
        ctx.tileHeightPx,
        shadowColor,
        0.65f);

    const Vector2 hullForward = Rotate({ 1.0f, 0.0f }, hullAngleRad);
    const Vector2 hullRight = Rotate({ 0.0f, 1.0f }, hullAngleRad);

    const float hullForwardPx = 18.0f;
    const float hullBackPx = 12.0f;
    const float hullHalfWidthPx = 10.0f;

    const SDL_FPoint hullA{
        centerPx.x + hullForward.x * hullForwardPx,
        centerPx.y + hullForward.y * hullForwardPx
    };
    const SDL_FPoint hullB{
        centerPx.x - hullForward.x * hullBackPx + hullRight.x * hullHalfWidthPx,
        centerPx.y - hullForward.y * hullBackPx + hullRight.y * hullHalfWidthPx
    };
    const SDL_FPoint hullC{
        centerPx.x - hullForward.x * hullBackPx - hullRight.x * hullHalfWidthPx,
        centerPx.y - hullForward.y * hullBackPx - hullRight.y * hullHalfWidthPx
    };

    RenderHelper::DrawTriangleThick(
        renderer,
        hullA,
        hullB,
        hullC,
        3.0f,
        SDL_FColor{ 80.0f / 255.0f, 200.0f / 255.0f, 120.0f / 255.0f, 1.0f });

    const Vector2 turretForward = Rotate({ 1.0f, 0.0f }, turretAngleRad);
    const Vector2 turretRight = Rotate({ 0.0f, 1.0f }, turretAngleRad);

    const float turretForwardPx = 10.0f;
    const float turretBackPx = 7.0f;
    const float turretHalfWidthPx = 6.0f;

    const SDL_FPoint turretA{
        centerPx.x + turretForward.x * turretForwardPx,
        centerPx.y + turretForward.y * turretForwardPx
    };
    const SDL_FPoint turretB{
        centerPx.x - turretForward.x * turretBackPx + turretRight.x * turretHalfWidthPx,
        centerPx.y - turretForward.y * turretBackPx + turretRight.y * turretHalfWidthPx
    };
    const SDL_FPoint turretC{
        centerPx.x - turretForward.x * turretBackPx - turretRight.x * turretHalfWidthPx,
        centerPx.y - turretForward.y * turretBackPx - turretRight.y * turretHalfWidthPx
    };

    RenderHelper::DrawTriangleThick(
        renderer,
        turretA,
        turretB,
        turretC,
        2.5f,
        SDL_FColor{ 40.0f / 255.0f, 140.0f / 255.0f, 220.0f / 255.0f, 1.0f });

    const float barrelLengthPx = 26.0f;
    const SDL_FPoint muzzle{
        centerPx.x + turretForward.x * barrelLengthPx,
        centerPx.y + turretForward.y * barrelLengthPx
    };
    RenderHelper::DrawThickLine(
        renderer,
        centerPx,
        muzzle,
        3.5f,
        SDL_FColor{ 40.0f / 255.0f, 140.0f / 255.0f, 220.0f / 255.0f, 1.0f });

    SDL_RenderPoint(renderer, centerPx.x, centerPx.y);
}
