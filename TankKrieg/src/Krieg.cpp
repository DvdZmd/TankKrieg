#include <iostream>

#include "Assets/TextureManager.h"
#include "Krieg.h"
#include "Render/RenderContext.h"
#include "Scene/IScene.h"
#include "Scene/PlayScene.h"

Krieg::Krieg() {}

Krieg::~Krieg() {
    // Defensive RAII cleanup if Shutdown is skipped in main.
    Shutdown();
}

bool Krieg::Initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Tank Krieg", windowWidthPx, windowHeightPx, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    textureManager = std::make_unique<TextureManager>(renderer);
    activeScene = std::make_unique<PlayScene>(windowWidthPx, windowHeightPx, *textureManager);
    activeScene->OnEnter();

    isRunning = true;
    return true;
}

void Krieg::Run() {
    Uint64 lastTick = SDL_GetTicks();

    while (isRunning) {
        Uint64 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        ProcessEvents();
        Update(deltaTime);
        Render();
    }
}

void Krieg::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (activeScene) {
            activeScene->HandleEvent(event);
        }
    }
}

void Krieg::Update(float deltaTime) {
    if (activeScene) {
        activeScene->Update(deltaTime);
        isRunning = !activeScene->WantsToQuit();
    }
}

void Krieg::Render() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    RenderContext ctx{};
    ctx.renderer = renderer;
    if (activeScene) {
        activeScene->Render(ctx);
    }

    SDL_RenderPresent(renderer);
}

/**
 * @brief Shut down input, rendering objects, and SDL subsystems in a safe order.
 */
void Krieg::Shutdown() {
    // Idempotent shutdown: safe to call more than once.
    if (activeScene) {
        activeScene->OnExit();
        activeScene.reset();
    }

    if (textureManager) {
        textureManager->Clear();
        textureManager.reset();
    }

    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window) { SDL_DestroyWindow(window);     window = nullptr; }

    if (SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Quit();
    }

    isRunning = false;
}

