#include "Krieg.h"
#include <iostream>

struct Point {
    float x, y;
};

static Point GridToScreen(int gx, int gy, int tileW, int tileH, int originX, int originY)
{
    float sx = originX + (gx - gy) * (tileW * 0.5f);
    float sy = originY + (gx + gy) * (tileH * 0.5f);
    return { sx, sy };
}

static void DrawIsoDiamond(SDL_Renderer* r, float cx, float cy, int tileW, int tileH)
{
    SDL_FPoint top = { cx,              cy - tileH * 0.5f };
    SDL_FPoint right = { cx + tileW * 0.5f, cy };
    SDL_FPoint bottom = { cx,              cy + tileH * 0.5f };
    SDL_FPoint left = { cx - tileW * 0.5f, cy };

    SDL_RenderLine(r, top.x, top.y, right.x, right.y);
    SDL_RenderLine(r, right.x, right.y, bottom.x, bottom.y);
    SDL_RenderLine(r, bottom.x, bottom.y, left.x, left.y);
    SDL_RenderLine(r, left.x, left.y, top.x, top.y);
}

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

    const int tileW = 64;
    const int tileH = 64 / 2;

    const int originX = 400; // centro de tu ventana (ajustalo)
    const int originY = 100; // un poco abajo del top para que se vea

    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);

    // dibujar una grilla 10x10
    for (int gy = 0; gy < 10; gy++) {
        for (int gx = 0; gx < 10; gx++) {
            Point p = GridToScreen(gx, gy, tileW, tileH, originX, originY);
            DrawIsoDiamond(renderer, p.x, p.y, tileW, tileH);
        }
    }


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
