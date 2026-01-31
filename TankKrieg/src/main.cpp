#include <SDL3/SDL.h>    // Recomendado para SDL3
#include <iostream>

int main(int argc, char** argv)
{
	/* initialize SDL */
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"Tank Krieg", 
		800, 600, 
		0);

	if (!window) {
		std::cerr << "Error creating Krieg Window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}


	bool isRunning = true;
	while (isRunning) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				isRunning = false;
			}
		}
		// Podremos agregar render aquí más adelante
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

