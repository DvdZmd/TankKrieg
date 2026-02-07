#include <cmath>
#include "Entities/Tank.h"
#include "Math/IsoUtils.h"
#include "Render/IsoDebugDraw.h"
#include <Render/Helper.h>


static Int2 SnapVisual8(const Vector2& v, float deadzone = 0.25f)
{
	const float lenSq = v.x * v.x + v.y * v.y;
	if (lenSq < deadzone * deadzone) return { 0, 0 };

	Int2 s{ 0,0 };
	s.x = (v.x > deadzone) ? 1 : (v.x < -deadzone ? -1 : 0);
	s.y = (v.y > deadzone) ? 1 : (v.y < -deadzone ? -1 : 0);
	return s;
}

static Vector2 NormalizeStep(const Int2& s)
{
	Vector2 v{ (float)s.x, (float)s.y };
	const float lsq = v.x * v.x + v.y * v.y;
	if (lsq < 0.000001f) return { 0.0f, 0.0f };
	const float inv = 1.0f / SDL_sqrtf(lsq);
	return { v.x * inv, v.y * inv };
}


static float LengthSq(const Vector2& v)
{
	return v.x * v.x + v.y * v.y;
}

static Vector2 NormalizeSafe(const Vector2& v)
{
	const float lsq = LengthSq(v);
	if (lsq < 0.000001f) return { 0.0f, 0.0f };
	const float inv = 1.0f / SDL_sqrtf(lsq);
	return { v.x * inv, v.y * inv };
}

static Vector2 Rotate(const Vector2& v, float a)
{
	const float c = std::cos(a);
	const float s = std::sin(a);
	return { v.x * c - v.y * s, v.x * s + v.y * c };
}

static void DrawTriangleOutline(SDL_Renderer* r, const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& c)
{
	SDL_RenderLine(r, a.x, a.y, b.x, b.y);
	SDL_RenderLine(r, b.x, b.y, c.x, c.y);
	SDL_RenderLine(r, c.x, c.y, a.x, a.y);
}

float Tank::AngleFromVectorVisual(const Vector2& v)
{
	// Visual/screen space: +x right, +y down -> atan2(y, x) matches SDL coordinates
	return std::atan2(v.y, v.x);
}

void Tank::SetGridPosition(float gx, float gy)
{
	gridX = gx;
	gridY = gy;
}

void Tank::Update(float dt, const Vector2& moveVisual, const Vector2& aimVisual, const GridBounds& bounds)
{
	// --- Hull movement: SNAP to 8 directions (visual), then map to grid
	const Int2 moveSnap = SnapVisual8(moveVisual);

	if (moveSnap.x != 0 || moveSnap.y != 0) {
		// Use snapped visual direction for hull angle (8-dir)
		const Vector2 mvSnapNorm = NormalizeStep(moveSnap);
		hullAngleRad = AngleFromVectorVisual(mvSnapNorm);

		// Convert snapped visual direction to grid step (8-dir grid step)
		const Int2 gridStep = IsoUtils::VisualToIsoGridStep(moveSnap);

		// Normalize grid step so diagonals don't move faster
		const Vector2 gridDir = NormalizeStep(gridStep);

		//gridX += gridDir.x * moveSpeedTilesPerSec * dt;
		//gridY += gridDir.y * moveSpeedTilesPerSec * dt;

		const float nextX = gridX + gridDir.x * moveSpeedTilesPerSec * dt;
		const float nextY = gridY + gridDir.y * moveSpeedTilesPerSec * dt;

		// X TODO IsWalkable there is no map
		//if (map.IsWalkable((int)nextX, (int)gridY))
		//	gridX = nextX;

		//// Y
		//if (map.IsWalkable((int)gridX, (int)nextY))
		//	gridY = nextY;

		gridX = Helper::ClampFloat(nextX, bounds.minX, bounds.maxX);
		gridY = Helper::ClampFloat(nextY, bounds.minY, bounds.maxY);


	}

	// --- Turret aiming: ANALOG (N directions). No snapping here.
	const float aimLenSq = aimVisual.x * aimVisual.x + aimVisual.y * aimVisual.y;
	if (aimLenSq > 0.0004f) {
		const Vector2 av = NormalizeSafe(aimVisual);
		turretAngleRad = AngleFromVectorVisual(av);
	}
}

void Tank::Render(SDL_Renderer* renderer, int tileW, int tileH, int originX, int originY) const
{
	const SDL_FPoint p = IsoUtils::GridToScreenF(gridX, gridY, tileW, tileH, originX, originY);

	// Shadow on ground (iso diamond)
	{
		const float shadowYOffset = tileH * 0.12f; // tweak visually
		const float shadowScale = 0.65f;

		// RGBA in 0..1 floats
		const SDL_FColor shadowColor{ 0.0f, 0.0f, 0.0f, 0.35f };

		IsoDebugDraw::FillIsoDiamond(renderer, p.x, p.y + shadowYOffset, tileW, tileH, shadowColor, shadowScale);
	}

	// --------------------
	// Hull (triangle outline)
	// --------------------
	const Vector2 hf = Rotate({ 1.0f, 0.0f }, hullAngleRad);
	const Vector2 hr = Rotate({ 0.0f, 1.0f }, hullAngleRad);

	const float hullForward = 18.0f;
	const float hullBack = 12.0f;
	const float hullHalfWidth = 10.0f;

	SDL_FPoint ha{ p.x + hf.x * hullForward,                 p.y + hf.y * hullForward };
	SDL_FPoint hb{ p.x - hf.x * hullBack + hr.x * hullHalfWidth, p.y - hf.y * hullBack + hr.y * hullHalfWidth };
	SDL_FPoint hc{ p.x - hf.x * hullBack - hr.x * hullHalfWidth, p.y - hf.y * hullBack - hr.y * hullHalfWidth };

	SDL_SetRenderDrawColor(renderer, 80, 200, 120, 255);
	//DrawTriangleOutline(renderer, ha, hb, hc);
	Helper::DrawTriangleThick(renderer, ha, hb, hc, 3.0f, SDL_FColor{ 80.0f / 255.0f, 200.0f / 255.0f, 120.0f / 255.0f, 1.0f });

	// --------------------
	// Turret (smaller triangle + barrel line)
	// --------------------
	const Vector2 tf = Rotate({ 1.0f, 0.0f }, turretAngleRad);
	const Vector2 tr = Rotate({ 0.0f, 1.0f }, turretAngleRad);

	const float turretForward = 10.0f;
	const float turretBack = 7.0f;
	const float turretHalfWidth = 6.0f;

	SDL_FPoint ta{ p.x + tf.x * turretForward,                  p.y + tf.y * turretForward };
	SDL_FPoint tb{ p.x - tf.x * turretBack + tr.x * turretHalfWidth, p.y - tf.y * turretBack + tr.y * turretHalfWidth };
	SDL_FPoint tc{ p.x - tf.x * turretBack - tr.x * turretHalfWidth, p.y - tf.y * turretBack - tr.y * turretHalfWidth };

	SDL_SetRenderDrawColor(renderer, 40, 140, 220, 255);
	//DrawTriangleOutline(renderer, ta, tb, tc);
	Helper::DrawTriangleThick(renderer, ta, tb, tc, 2.5f, SDL_FColor{ 40.0f / 255.0f, 140.0f / 255.0f, 220.0f / 255.0f, 1.0f });

	// Barrel
	const float barrelLen = 26.0f;
	SDL_FPoint muzzle{ p.x + tf.x * barrelLen, p.y + tf.y * barrelLen };
	//SDL_RenderLine(renderer, p.x, p.y, muzzle.x, muzzle.y);
	Helper::DrawThickLine(renderer, p, muzzle, 3.5f, SDL_FColor{ 40.0f / 255.0f, 140.0f / 255.0f, 220.0f / 255.0f, 1.0f });


	// Optional: center dot
	SDL_RenderPoint(renderer, p.x, p.y);
}
