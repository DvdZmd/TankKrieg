#include <array>
#include <cmath>
#include <memory>

#include "Scene/PlayScene.h"

#include "Assets/TextureManager.h"
#include "Math/IsoUtils.h"
#include "Render/RenderContext.h"

namespace
{
    constexpr const char* kTankHullTextureKey = "tank.player.hull";
    constexpr const char* kTankTurretTextureKey = "tank.player.turret";
    constexpr const char* kTankHullTexturePath = "assets/textures/tank_hull.bmp";
    constexpr const char* kTankTurretTexturePath = "assets/textures/tank_turret.bmp";

    std::array<SDL_FRect, DirectionalSpriteSet::kDirectionCount> BuildHorizontalFrameSet(SDL_Texture* texture)
    {
        std::array<SDL_FRect, DirectionalSpriteSet::kDirectionCount> frames{};
        if (texture == nullptr)
        {
            return frames;
        }

        float textureWidth = 0.0f;
        float textureHeight = 0.0f;
        if (!SDL_GetTextureSize(texture, &textureWidth, &textureHeight) || textureWidth <= 0.0f || textureHeight <= 0.0f)
        {
            return frames;
        }

        const bool hasEightFrames = std::fmod(textureWidth, static_cast<float>(DirectionalSpriteSet::kDirectionCount)) == 0.0f;
        const float frameWidth = hasEightFrames
            ? textureWidth / static_cast<float>(DirectionalSpriteSet::kDirectionCount)
            : textureWidth;

        for (int index = 0; index < DirectionalSpriteSet::kDirectionCount; ++index)
        {
            const float x = hasEightFrames ? frameWidth * static_cast<float>(index) : 0.0f;
            frames[index] = SDL_FRect{ x, 0.0f, frameWidth, textureHeight };
        }

        return frames;
    }

    DirectionalSpriteSet BuildDirectionalLayer(SDL_Texture* texture)
    {
        DirectionalSpriteSet layer{};
        layer.texture = texture;
        if (texture == nullptr)
        {
            return layer;
        }

        float textureWidth = 0.0f;
        float textureHeight = 0.0f;
        if (!SDL_GetTextureSize(texture, &textureWidth, &textureHeight) || textureWidth <= 0.0f || textureHeight <= 0.0f)
        {
            return layer;
        }

        const bool hasEightFrames = std::fmod(textureWidth, static_cast<float>(DirectionalSpriteSet::kDirectionCount)) == 0.0f;
        const float frameWidth = hasEightFrames
            ? textureWidth / static_cast<float>(DirectionalSpriteSet::kDirectionCount)
            : textureWidth;

        layer.sourceRects = BuildHorizontalFrameSet(texture);
        layer.sizePx = { frameWidth, textureHeight };
        layer.originPx = { frameWidth * 0.5f, textureHeight * 0.8f };
        return layer;
    }
}

PlayScene::PlayScene(int viewportWidthPx, int viewportHeightPx, TextureManager& textureManagerRef)
    : textureManager(textureManagerRef)
{
    camera.SetViewportSize(viewportWidthPx, viewportHeightPx);
}

void PlayScene::OnEnter()
{
    input.Initialize();
    InitializeTestBlockedTiles();
    BuildDefaultTankVisualDefinition();
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
        tankMovementResolver.ApplyMovement(*playerTank, deltaTime, tileMap);
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
    worldRenderer.Render(world, sceneContext, playerTank, &playerTankVisual);
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

void PlayScene::BuildDefaultTankVisualDefinition()
{
    playerTankVisual = {};

    SDL_Texture* hullTexture = textureManager.Load(kTankHullTextureKey, kTankHullTexturePath);
    SDL_Texture* turretTexture = textureManager.Load(kTankTurretTextureKey, kTankTurretTexturePath);

    playerTankVisual.hull = BuildDirectionalLayer(hullTexture);
    playerTankVisual.turret = BuildDirectionalLayer(turretTexture);
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
