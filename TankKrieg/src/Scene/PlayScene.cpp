#include <memory>

#include "Scene/PlayScene.h"

#include "Math/IsoUtils.h"
#include "Render/RenderContext.h"
#include "Render/WorldVisualRegistry.h"

PlayScene::PlayScene(int viewportWidthPx, int viewportHeightPx, TextureManager& textureManagerRef)
    : textureManager(textureManagerRef)
{
    camera.SetViewportSize(viewportWidthPx, viewportHeightPx);
}

void PlayScene::OnEnter()
{
    input.Initialize();
    InitializeTestBlockedTiles();
    playerTankVisual = tankVisualFactory.BuildDefault(textureManager);
    worldVisualRegistry.Clear();
    EnsurePlayerTank();
    UpdateCamera();
}

void PlayScene::OnExit()
{
    input.Shutdown();
    world.Clear();
    worldVisualRegistry.Clear();
    playerTank = nullptr;
}

void PlayScene::HandleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_WINDOW_RESIZED) {
        camera.SetViewportSize(event.window.data1, event.window.data2);
        return;
    }

    input.HandleEvent(event);
}

void PlayScene::Update(float deltaTime)
{
    if (playerTank != nullptr && !world.Contains(playerTank))
    {
        playerTank = nullptr;
    }

    input.Update(deltaTime);

    const Vector2 moveVisual = input.GetMovementVector();
    Vector2 moveGridTiles = IsoUtils::VisualDirToGridDir(moveVisual, kTileWidthPx, kTileHeightPx);
    moveGridTiles = IsoUtils::SnapGridDir8(moveGridTiles);

    debugCrosshairGridTiles.x += moveGridTiles.x * deltaTime * 2.5f;
    debugCrosshairGridTiles.y += moveGridTiles.y * deltaTime * 2.5f;

    cursorGridTile.x += input.GetCursorStep().x;
    cursorGridTile.y += input.GetCursorStep().y;
    ClampCursorToMapBounds();

    if (playerTank) {
        playerTank->SetMovementIntent(TankMovementIntent::FromVisualDirection(moveVisual));
        playerTank->SetAimVisual(input.GetAimVector());
        tankMovementResolver.ApplyMovement(*playerTank, playerTank->GetMovementIntent(), deltaTime, tileMap);
    }

    debugOverlay.SetCursorTile(cursorGridTile);
    debugOverlay.SetCrosshairGridTiles(debugCrosshairGridTiles);

    world.Update(deltaTime);
    UpdateCamera();
}

void PlayScene::Render(const RenderContext& renderContext)
{
    RenderContext sceneContext = renderContext;
    sceneContext.tileWidthPx = kTileWidthPx;
    sceneContext.tileHeightPx = kTileHeightPx;
    sceneContext.originXPx = camera.OriginXpx();
    sceneContext.originYPx = camera.OriginYpx();

    tileMapRenderer.Render(sceneContext, tileMap);
    worldRenderer.Render(world, sceneContext, worldVisualRegistry);
    debugOverlay.Render(sceneContext);
}

bool PlayScene::WantsToQuit() const
{
    return input.QuitRequested();
}

void PlayScene::ClampCursorToMapBounds()
{
    if (cursorGridTile.x < 0) cursorGridTile.x = 0;
    if (cursorGridTile.y < 0) cursorGridTile.y = 0;
    if (cursorGridTile.x >= tileMap.Width()) cursorGridTile.x = tileMap.Width() - 1;
    if (cursorGridTile.y >= tileMap.Height()) cursorGridTile.y = tileMap.Height() - 1;
}

void PlayScene::UpdateCamera()
{
    if (playerTank != nullptr && !world.Contains(playerTank))
    {
        playerTank = nullptr;
    }

    if (!playerTank) {
        return;
    }

    camera.SetTargetGrid(playerTank->GetGridPosition());
    camera.Update(kTileWidthPx, kTileHeightPx);
}

void PlayScene::InitializeTestBlockedTiles()
{
    for (int x = 7; x <= 11; ++x)
    {
        tileMap.SetBlocksMovement({ x, 8 }, true);
    }

    for (int y = 10; y <= 12; ++y)
    {
        tileMap.SetBlocksMovement({ 10, y }, true);
        tileMap.SetBlocksMovement({ 11, y }, true);
    }
}

void PlayScene::EnsurePlayerTank()
{
    if (playerTank) {
        return;
    }

    auto tank = std::make_unique<Tank>();
    tank->SetGridPosition(4.0f, 4.0f);
    playerTank = static_cast<Tank*>(world.Add(std::move(tank)));
    worldVisualRegistry.Bind(playerTank, WorldVisualDefinitionBinding::Tank(&playerTankVisual));
}
