#pragma once

#include "Render/TankVisualDefinition.h"

class TextureManager;

class TankVisualFactory
{
public:
    TankVisualDefinition BuildDefault(TextureManager& textureManager) const;
};
