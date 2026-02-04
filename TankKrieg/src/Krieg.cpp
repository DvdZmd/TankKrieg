#include "Krieg.h"
#include "Math/IsoUtils.h"
#include "Render/IsoDebugDraw.h"
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

    window = SDL_CreateWindow("Tank Krieg", kriegWidth, kriegHeight, SDL_WINDOW_RESIZABLE);
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

        ProcessInput(deltaTime);
        Update(deltaTime);
        Render();
    }
}

void Krieg::ProcessInput(float deltaTime) {
    input.Update(deltaTime);
    if (input.QuitRequested()) {
        isRunning = false;
    }
}

void Krieg::Update(float deltaTime) {
    // Sandbox: move the grid cursor one tile per key press (or D-pad press).
    Int2 step = input.GetCursorStep();
    cursorX += step.x;
    cursorY += step.y;

    // Clamp cursor inside grid bounds
    if (cursorX < 0) cursorX = 0;
    if (cursorY < 0) cursorY = 0;
    if (cursorX >= gridW) cursorX = gridW - 1;
	if (cursorY >= gridH) cursorY = gridH - 1;
}

void Krieg::Render() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    const int tileW = tileWidth;
    const int tileH = tileHeight;

	//pixel coordinates of the origin (0,0) grid tile
    const int originX = 400;
    const int originY = 100;

    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);

    // Draw base grid
    for (int gy = 0; gy < gridH; gy++) {
        for (int gx = 0; gx < gridW; gx++) {
            SDL_FPoint p = IsoUtils::GridToScreen(gx, gy, tileW, tileH, originX, originY);
            IsoDebugDraw::DrawIsoDiamondOutline(renderer, p.x, p.y, tileW, tileH);
        }
    }

    // Highlight selected tile (cursor)
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
    SDL_FPoint c = IsoUtils::GridToScreen(cursorX, cursorY, tileW, tileH, originX, originY);
    // relleno rojo semitransparente
    IsoDebugDraw::FillIsoDiamond(renderer, c.x, c.y, tileW, tileH);

    // borde rojo más fuerte arriba (opcional)
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
    IsoDebugDraw::DrawIsoDiamondOutline(renderer, c.x, c.y, tileW, tileH);


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
