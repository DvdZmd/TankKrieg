#pragma once

#include <memory>

#include "Scene/IScene.h"
#include "Math/Int2.h"
#include "Math/Vector2.h"
#include "World/World.h"
#include "World/TileMap.h"
#include "World/TankMovementResolver.h"
#include "Input/InputManager.h"
#include "Entities/Tank.h"
#include "Render/Camera2D.h"
#include "Render/TileMapRenderer.h"
#include "Render/DebugOverlay.h"
#include "Render/TankVisualFactory.h"
#include "Render/TankVisualDefinition.h"
#include "Render/WorldVisualRegistry.h"
#include "Render/WorldRenderer.h"

class TextureManager;

class PlayScene final : public IScene
{
public:
    PlayScene(int viewportWidthPx, int viewportHeightPx, TextureManager& textureManager);

    void OnEnter() override;
    void OnExit() override;

    void HandleEvent(const SDL_Event& event) override;
    void Update(float deltaTime) override;
    void Render(const RenderContext& renderContext) override;
    bool WantsToQuit() const override;

private:
    void ClampCursorToMapBounds();
    void UpdateCamera();
    void InitializeTestBlockedTiles();
    void EnsurePlayerTank();

private:
    static constexpr int kTileWidthPx = 64;
    static constexpr int kTileHeightPx = kTileWidthPx / 2;

    World world;
    TileMap tileMap{ 20, 20 };
    InputManager input;
    Camera2D camera;
    TileMapRenderer tileMapRenderer;
    WorldRenderer worldRenderer;
    DebugOverlay debugOverlay;
    TextureManager& textureManager;
    TankMovementResolver tankMovementResolver;
    TankVisualFactory tankVisualFactory;

    Tank* playerTank = nullptr;
    TankVisualDefinition playerTankVisual{};
    WorldVisualRegistry worldVisualRegistry;
    Int2 cursorGridTile{ 0, 0 };
    Vector2 debugCrosshairGridTiles{ 0.0f, 0.0f };
};
