#include <SDL3/SDL.h>

class Krieg {
public:
	Krieg();
	~Krieg();
	bool Initialize();
	void Run();
	void Shutdown();
private:
	void ProcessInput();
	void Update(float deltaTime);
	void Render();

private:
	bool isRunning;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};