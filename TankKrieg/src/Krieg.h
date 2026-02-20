#pragma once
#include <SDL3/SDL.h>
#include "Math/Int2.h"
#include "Math/Vector2.h"
#include "World/World.h"
#include "World/TileMap.h"
#include "Input/InputManager.h"
#include "Entities/Tank.h"
#include "Render/TileMapRenderer.h"
#include "Render/DebugOverlay.h"

class Krieg {
public:
	Krieg();
	~Krieg();

	bool Initialize();
	void Run();
	void Shutdown();

private:
	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
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

	int windowWidthPx = 800;
	int windowHeightPx = 600;

	TileMap tileMap{ 10, 10 };
	TileMapRenderer tileMapRenderer;
	DebugOverlay debugOverlay;

	Int2 cursorGridTile{ 0, 0 };
	Vector2 debugCrosshairGridTiles{ 0.0f, 0.0f };
};
