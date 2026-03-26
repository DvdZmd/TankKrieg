#include <memory>

#include "Scene/PlayScene.h"

#include "Math/IsoUtils.h"
#include "Render/RenderContext.h"

PlayScene::PlayScene(int viewportWidthPx, int viewportHeightPx)
{
    camera.SetViewportSize(viewportWidthPx, viewportHeightPx);
}

void PlayScene::OnEnter()
{
    input.Initialize();
    EnsurePlayerTank();
    UpdateCamera();
}

void PlayScene::OnExit()
{
    input.Shutdown();
    world.Clear();
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
        playerTank->SetMoveVisual(moveVisual);
        playerTank->SetAimVisual(input.GetAimVector());
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
    world.Render(sceneContext);
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

void PlayScene::EnsurePlayerTank()
{
    if (playerTank) {
        return;
    }

    auto tank = std::make_unique<Tank>();
    tank->SetGridPosition(4.0f, 4.0f);
    playerTank = static_cast<Tank*>(world.Add(std::move(tank)));
}
