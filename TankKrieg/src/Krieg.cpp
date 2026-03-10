#include <iostream>
#include <memory>
#include "Krieg.h"
#include "Math/IsoUtils.h"
#include "Render/RenderContext.h"

Krieg::Krieg() {}

Krieg::~Krieg() {
    // Defensive RAII cleanup if Shutdown is skipped in main.
    Shutdown();
}

/// <summary>
/// Initializes the game: sets up SDL, creates the window and renderer, initializes input, camera, and player tank.
/// </summary>
/// <returns>True if initialization succeeds; false otherwise.</returns>
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

    input.Initialize(); // SDL is initialized at this point.
    camera.SetViewportSize(windowWidthPx, windowHeightPx);
    // TODO: update viewport size when handling resize events.

    // Initialize the player tank and add it to the world.
    auto tank = std::make_unique<Tank>();
	tank->position = { 4.0f, 4.0f };
	playerTank = static_cast<Tank*>(world.Add(std::move(tank)));

    isRunning = true;
    return true;
}

/// <summary>
/// Main game loop: calculates delta time, processes input, updates game state, and renders the scene.
/// </summary>
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

/// <summary>
/// Processes player input: updates input state, converts visual movement to grid-space movement to update a debug crosshair, and handles quit requests.
/// </summary>
/// <param name="deltaTime">Elapsed time in seconds since the last update; used to scale time-dependent movement of the debug crosshair.</param>
void Krieg::ProcessInput(float deltaTime) {
    input.Update(deltaTime);

    // Continuous movement debug crosshair in grid space (tiles).
    Vector2 moveVisual = input.GetMovementVector(); // left stick, visual space
    Vector2 moveGridTiles = IsoUtils::VisualDirToGridDir(moveVisual, tileWidthPx, tileHeightPx);

    moveGridTiles = IsoUtils::SnapGridDir8(moveGridTiles);

    debugCrosshairGridTiles.x += moveGridTiles.x * deltaTime * 2.5f;
    debugCrosshairGridTiles.y += moveGridTiles.y * deltaTime * 2.5f;

    if (input.QuitRequested()) {
        isRunning = false;
    }
}

/// <summary>
/// Performs per-frame game updates: processes input, moves and clamps the cursor, applies player input intents, updates all world entities and debug overlay, and updates the camera.
/// </summary>
/// <param name="deltaTime">Elapsed time since the last update, in seconds; used to advance the simulation.</param>
void Krieg::Update(float deltaTime) {
    // Move the cursor one tile per key press or D-pad press.
    Int2 stepGridTiles = input.GetCursorStep();
    cursorGridTile.x += stepGridTiles.x;
    cursorGridTile.y += stepGridTiles.y;

    // Clamp cursor inside grid bounds.
    if (cursorGridTile.x < 0) cursorGridTile.x = 0;
    if (cursorGridTile.y < 0) cursorGridTile.y = 0;
    if (cursorGridTile.x >= tileMap.Width()) cursorGridTile.x = tileMap.Width() - 1;
	if (cursorGridTile.y >= tileMap.Height()) cursorGridTile.y = tileMap.Height() - 1;


    // 1) Read input and feed the player tank "intent"
    const Vector2 moveVisual = input.GetMovementVector(); // left stick
    const Vector2 aimVisual = input.GetAimVector();      // right stick
    if (playerTank)
    {
        playerTank->SetMoveVisual(moveVisual);
        playerTank->SetAimVisual(aimVisual);
    }

    debugOverlay.SetCursorTile(cursorGridTile);
    debugOverlay.SetCrosshairGridTiles(debugCrosshairGridTiles);

    // 2) Update all entities
    world.Update(deltaTime);

    if (playerTank)
    {
        camera.SetTargetGrid(playerTank->position);
        camera.Update(tileWidthPx, tileHeightPx);
    }
}

/// <summary>
/// Renders the current scene to the object's SDL renderer: clears the screen with a background color, builds a RenderContext from the object's state, invokes the tile map, world, and debug overlay renderers, and presents the rendered frame.
/// </summary>
void Krieg::Render() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    RenderContext ctx{};
    ctx.renderer = renderer;
    ctx.tileWidthPx = tileWidthPx;
    ctx.tileHeightPx = tileHeightPx;
    ctx.originXPx = camera.OriginXpx();
    ctx.originYPx = camera.OriginYpx();

    tileMapRenderer.Render(ctx, tileMap);
    world.Render(ctx);
    debugOverlay.Render(ctx);

    SDL_RenderPresent(renderer);
}

void Krieg::Shutdown() {
    // Idempotent shutdown: safe to call more than once.
    input.Shutdown();

    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window) { SDL_DestroyWindow(window);     window = nullptr; }

    if (SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Quit();
    }

    isRunning = false;
}

