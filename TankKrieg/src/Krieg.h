#pragma once
#include <memory>

#include <SDL3/SDL.h>

class IScene;

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
	 * @brief Poll SDL events and forward them to the active scene.
	 */
	void ProcessEvents();
	/**
	 * @brief Advance the active scene by one frame.
	 * @param deltaTime Elapsed simulation time in seconds for the current frame.
	 */
	void Update(float deltaTime);
	/**
	 * @brief Render the current frame using the active SDL renderer.
	 */
	void Render();

private:
	bool isRunning = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int windowWidthPx = 1280;
	int windowHeightPx = 920;
	std::unique_ptr<IScene> activeScene;
};
