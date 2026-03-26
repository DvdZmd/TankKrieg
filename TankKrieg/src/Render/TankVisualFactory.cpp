#include "Render/TankVisualFactory.h"

#include "Assets/TextureManager.h"
#include "Render/DirectionalSpriteFactory.h"
#include "Render/TankVisualDefinition.h"

namespace
{
    constexpr const char* kTankHullTextureKey = "tank.player.hull";
    constexpr const char* kTankTurretTextureKey = "tank.player.turret";
    constexpr const char* kTankHullTexturePath = "assets/textures/tank_hull.bmp";
    constexpr const char* kTankTurretTexturePath = "assets/textures/tank_turret.bmp";
}

TankVisualDefinition TankVisualFactory::BuildDefault(TextureManager& textureManager) const
{
    TankVisualDefinition visualDefinition{};

    SDL_Texture* hullTexture = textureManager.Load(kTankHullTextureKey, kTankHullTexturePath);
    SDL_Texture* turretTexture = textureManager.Load(kTankTurretTextureKey, kTankTurretTexturePath);

    visualDefinition.hull = DirectionalSpriteFactory::BuildHorizontalStrip(hullTexture);
    visualDefinition.turret = DirectionalSpriteFactory::BuildHorizontalStrip(turretTexture);
    return visualDefinition;
}
