#pragma once
#include <SDL3/SDL.h>
#include "Input/InputManager.h"
#include "Entities/Tank.h"

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

	Tank playerTank;

	bool isRunning = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	InputManager input;

	//tiles size
	int tileWidth = 64;
	int tileHeight = 64 / 2;

	//Windows resolution
	int kriegWidth = 800;	
	int kriegHeight = 600;

	// Sandbox: isometric grid cursor (tile highlight) TODO: If this growsup should be a class
	int cursorX = 0;
	int cursorY = 0;
	static constexpr int gridW = 10;
	static constexpr int gridH = 10;

	// DEBUG
	float debugGX = 0.0f;
	float debugGY = 0.0f;
};