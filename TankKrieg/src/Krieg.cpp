#include "Krieg.h"
#include "Math/IsoUtils.h"
#include "Render/IsoDebugDraw.h"
#include "Render/RenderContext.h"
#include <iostream>

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

    input.Initialize(); // SDL is initialized at this point.

    playerTank.SetGridPosition(4.0f, 4.0f);
    playerTank.SetMoveSpeed(2.5f);

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

    // Example: continuous movement test
    Vector2 moveVisual = input.GetMovementVector(); // left stick, visual space
    Vector2 moveGrid = IsoUtils::VisualDirToGridDir(moveVisual, tileWidth, tileHeight);

    // Optional: force 8-way
    moveGrid = IsoUtils::SnapGridDir8(moveGrid);

    debugGX += moveGrid.x * deltaTime * 2.5f; // tiles per second
    debugGY += moveGrid.y * deltaTime * 2.5f;

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


    // Tank Player 
    const Vector2 move = input.GetMovementVector(); // left stick
    const Vector2 aim = input.GetAimVector();      // right stick

    playerTank.SetMoveVisual(move);
    playerTank.SetAimVisual(aim);
    playerTank.Update(deltaTime);
}

void Krieg::Render() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    const int tileW = tileWidth;
    const int tileH = tileHeight;

	//pixel coordinates of the origin (0,0) grid tile
    const int originX = 400;
    const int originY = 100;

    // Draw base grid (filled tiles + dark edges)
    {
        // First pass: fill all tiles in white
        const SDL_FColor tileFill{ 1.0f, 1.0f, 1.0f, 1.0f }; // solid white
        for (int gy = 0; gy < gridH; ++gy) {
            for (int gx = 0; gx < gridW; ++gx) {
                SDL_FPoint p = IsoUtils::GridToScreen(gx, gy, tileW, tileH, originX, originY);
                IsoDebugDraw::FillIsoDiamond(renderer, p.x, p.y, tileW, tileH, tileFill, 1.0f);
            }
        }

        // Second pass: draw edges in dark gray on top
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); // dark gray edges
        for (int gy = 0; gy < gridH; ++gy) {
            for (int gx = 0; gx < gridW; ++gx) {
                SDL_FPoint p = IsoUtils::GridToScreen(gx, gy, tileW, tileH, originX, originY);
                IsoDebugDraw::DrawIsoDiamondOutline(renderer, p.x, p.y, tileW, tileH);
            }
        }
    }

    RenderContext ctx{};
    ctx.renderer = renderer;
    ctx.tileW = tileW;
    ctx.tileH = tileH;
    ctx.originX = originX;
    ctx.originY = originY;
    playerTank.Render(ctx);

    // Highlight selected tile (cursor)
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
    SDL_FPoint c = IsoUtils::GridToScreen(cursorX, cursorY, tileW, tileH, originX, originY);
    // relleno rojo semitransparente
    IsoDebugDraw::FillIsoDiamond(renderer, c.x, c.y, tileW, tileH);

    // stronger red outline on top (optional)
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
    IsoDebugDraw::DrawIsoDiamondOutline(renderer, c.x, c.y, tileW, tileH);


	//This is for testing, creates a cursor in the screen that moves in the isometric grid
    SDL_FPoint p = IsoUtils::GridToScreenF(debugGX, debugGY, tileW, tileH, originX, originY);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderLine(renderer, p.x - 5, p.y, p.x + 5, p.y);
    SDL_RenderLine(renderer, p.x, p.y - 5, p.x, p.y + 5);





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

