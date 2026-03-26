#pragma once
#include <SDL3/SDL.h>
#include "Math/Int2.h"
#include "Math/Vector2.h"
#include "World/World.h"
#include "World/TileMap.h"
#include "Input/InputManager.h"
#include "Entities/Tank.h"
#include "Render/Camera2D.h"
#include "Render/TileMapRenderer.h"
#include "Render/DebugOverlay.h"

class Krieg {
public:
	/**
	 * @brief Constructs the main game application object.
	 */
	Krieg();
	/**
	 * @brief Releases application resources that remain allocated at shutdown.
	 */
	~Krieg();

	/**
	 * @brief Initialize SDL, core systems, and the initial gameplay state.
	 * @return True when initialization succeeds; otherwise false.
	 */
	bool Initialize();
	/**
	 * @brief Execute the main game loop until the application stops running.
	 */
	void Run();
	/**
	 * @brief Shut down the application and release SDL resources.
	 */
	void Shutdown();

private:
	/**
	 * @brief Poll input sources and update frame-specific input state.
	 * @param deltaTime Elapsed time in seconds since the previous frame.
	 */
	void ProcessInput(float deltaTime);
	/**
	 * @brief Advance gameplay state, debug state, and camera tracking for one frame.
	 * @param deltaTime Elapsed simulation time in seconds for the current frame.
	 */
	void Update(float deltaTime);
	/**
	 * @brief Render the current frame using the active SDL renderer.
	 */
	void Render();

private:

	Tank* playerTank = nullptr;

	World world;

	bool isRunning = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	InputManager input;

	int tileWidthPx = 64;
	int tileHeightPx = 64 / 2;

	int windowWidthPx = 1280;
	int windowHeightPx = 920;

	Camera2D camera;

	TileMap tileMap{ 20, 20 };
	TileMapRenderer tileMapRenderer;
	DebugOverlay debugOverlay;

	Int2 cursorGridTile{ 0, 0 };
	Vector2 debugCrosshairGridTiles{ 0.0f, 0.0f };
};
