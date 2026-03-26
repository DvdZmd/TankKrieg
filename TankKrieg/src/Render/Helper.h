#pragma once
#include <SDL3/SDL.h>

namespace RenderHelper
{
	/**
	 * @brief Draw a thick line segment by rendering a quad between two points.
	 * @param renderer Renderer that receives the draw calls.
	 * @param a Start point of the line segment.
	 * @param b End point of the line segment.
	 * @param thickness Line thickness in pixels.
	 * @param color Color applied to the generated geometry.
	 */
	void DrawThickLine(
		SDL_Renderer* renderer,
		const SDL_FPoint& a,
		const SDL_FPoint& b,
		float thickness,
		const SDL_FColor& color);

	/**
	 * @brief Draw a triangle outline using thick line segments.
	 * @param renderer Renderer that receives the draw calls.
	 * @param a First triangle vertex.
	 * @param b Second triangle vertex.
	 * @param c Third triangle vertex.
	 * @param thickness Outline thickness in pixels.
	 * @param color Color applied to the generated geometry.
	 */
	void DrawTriangleThick(
		SDL_Renderer* renderer,
		const SDL_FPoint& a,
		const SDL_FPoint& b,
		const SDL_FPoint& c,
		float thickness,
		const SDL_FColor& color);
}
