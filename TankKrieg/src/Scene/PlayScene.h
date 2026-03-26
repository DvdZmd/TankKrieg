#pragma once

#include <memory>

#include "Scene/IScene.h"
#include "Math/Int2.h"
#include "Math/Vector2.h"
#include "World/World.h"
#include "World/TileMap.h"
#include "Input/InputManager.h"
#include "Entities/Tank.h"
#include "Entities/Entity.h"
#include "Render/TankVisual.h"
#include "Render/TankRenderer.h"
#include "Render/Camera2D.h"
#include "Render/TileMapRenderer.h"
#include "Render/DebugOverlay.h"

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
    Int2 WorldPositionToTile(const Vector2& worldPosition) const;
    bool CanEnterTile(const Int2& tile) const;
    void ApplyPlayerMovement(float deltaTime);
    void ClampCursorToMapBounds();
    void UpdateCamera();
    void InitializeTestBlockedTiles();
    void BuildDefaultTankVisualDefinition();
    void EnsurePlayerTank();
    void RenderWorld(const RenderContext& renderContext) const;
    void RenderEntity(const Entity& entity, const RenderContext& renderContext) const;
    const TankVisual* ResolveTankVisual(const Tank& tank) const;

private:
    static constexpr int kTileWidthPx = 64;
    static constexpr int kTileHeightPx = kTileWidthPx / 2;

    World world;
    TileMap tileMap{ 20, 20 };
    InputManager input;
    Camera2D camera;
    TileMapRenderer tileMapRenderer;
    TankRenderer tankRenderer;
    DebugOverlay debugOverlay;
    TextureManager& textureManager;

    Tank* playerTank = nullptr;
    TankVisual playerTankVisual{};
    Int2 cursorGridTile{ 0, 0 };
    Vector2 debugCrosshairGridTiles{ 0.0f, 0.0f };
};
