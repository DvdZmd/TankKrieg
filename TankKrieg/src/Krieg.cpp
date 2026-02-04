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

static void FillIsoDiamond(SDL_Renderer* r, float cx, float cy, int tileW, int tileH)
{
    SDL_Vertex verts[4];

    SDL_FPoint top = { cx,              cy - tileH * 0.5f };
    SDL_FPoint right = { cx + tileW * 0.5f, cy };
    SDL_FPoint bottom = { cx,              cy + tileH * 0.5f };
    SDL_FPoint left = { cx - tileW * 0.5f, cy };

    // SDL_Vertex: position + color + tex_coord (tex_coord puede ser 0)
    verts[0].position = top;    verts[0].tex_coord = { 0,0 };
    verts[1].position = right;  verts[1].tex_coord = { 0,0 };
    verts[2].position = bottom; verts[2].tex_coord = { 0,0 };
    verts[3].position = left;   verts[3].tex_coord = { 0,0 };

    // El color viene del renderer? No. En RenderGeometry el color es por vértice.
    // Lo setearás desde afuera antes llamando SDL_SetRenderDrawColor?
    // No afecta RenderGeometry. Hay que setearlo en cada vértice.
    // Por eso la función asume que querés rojo sólido:
    SDL_FColor c = { 1.0f, 0.0f, 0.0f, 0.35f }; // rojo con alpha (0.35)
    for (int i = 0; i < 4; ++i) verts[i].color = c;

    // 2 triángulos: (top,right,bottom) y (top,bottom,left)
    int indices[6] = { 0, 1, 2,  0, 2, 3 };

    SDL_RenderGeometry(r, nullptr, verts, 4, indices, 6);
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
    for (int gy = 0; gy < gridW; gy++) {
        for (int gx = 0; gx < gridH; gx++) {
            Point p = GridToScreen(gx, gy, tileW, tileH, originX, originY);
            DrawIsoDiamond(renderer, p.x, p.y, tileW, tileH);
        }
    }

    // Highlight selected tile (cursor)
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
    Point c = GridToScreen(cursorX, cursorY, tileW, tileH, originX, originY);
    // relleno rojo semitransparente
    FillIsoDiamond(renderer, c.x, c.y, tileW, tileH);

    // borde rojo más fuerte arriba (opcional)
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
    DrawIsoDiamond(renderer, c.x, c.y, tileW, tileH);


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
