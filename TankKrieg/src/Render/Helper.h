#pragma once
#include <SDL3/SDL.h>

namespace RenderHelper
{
	void DrawThickLine(
		SDL_Renderer* renderer,
		const SDL_FPoint& a,
		const SDL_FPoint& b,
		float thickness,
		const SDL_FColor& color);

	void DrawTriangleThick(
		SDL_Renderer* renderer,
		const SDL_FPoint& a,
		const SDL_FPoint& b,
		const SDL_FPoint& c,
		float thickness,
		const SDL_FColor& color);
}
