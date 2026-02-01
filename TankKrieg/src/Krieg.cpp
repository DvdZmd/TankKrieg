#include "Krieg.h"
#include <iostream>

Krieg::Krieg() {}

Krieg::~Krieg() {
    // RAII defensivo: si te olvidás llamar Shutdown en main, igual limpia.
    Shutdown();
}

bool Krieg::Initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Tank Krieg", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    input.Initialize(); // <- ahora sí, SDL ya está inicializado

    isRunning = true;
    return true;
}

void Krieg::Run() {
    Uint64 lastTick = SDL_GetTicks();

    while (isRunning) {
        Uint64 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        ProcessInput();
        Update(deltaTime);
        Render();
    }
}

void Krieg::ProcessInput() {
    input.Update();
    if (input.QuitRequested()) {
        isRunning = false;
    }
}

void Krieg::Update(float /*deltaTime*/) {
    // World update (tanks, AI later)
}

void Krieg::Render() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

void Krieg::Shutdown() {
    // idempotente: que no rompa si se llama 2 veces
    input.Shutdown();

    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window) { SDL_DestroyWindow(window);     window = nullptr; }

    if (SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Quit();
    }

    isRunning = false;
}
