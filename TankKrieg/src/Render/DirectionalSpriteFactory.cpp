#include <cmath>

#include "Render/DirectionalSpriteFactory.h"

DirectionalSpriteSet DirectionalSpriteFactory::BuildHorizontalStrip(SDL_Texture* texture, const Vector2& originFactor)
{
    DirectionalSpriteSet spriteSet{};
    spriteSet.texture = texture;
    if (texture == nullptr)
    {
        return spriteSet;
    }

    float textureWidth = 0.0f;
    float textureHeight = 0.0f;
    if (!SDL_GetTextureSize(texture, &textureWidth, &textureHeight) || textureWidth <= 0.0f || textureHeight <= 0.0f)
    {
        return spriteSet;
    }

    const bool hasEightFrames = std::fmod(textureWidth, static_cast<float>(DirectionalSpriteSet::kDirectionCount)) == 0.0f;
    const float frameWidth = hasEightFrames
        ? textureWidth / static_cast<float>(DirectionalSpriteSet::kDirectionCount)
        : textureWidth;

    for (int index = 0; index < DirectionalSpriteSet::kDirectionCount; ++index)
    {
        const float x = hasEightFrames ? frameWidth * static_cast<float>(index) : 0.0f;
        spriteSet.sourceRects[index] = SDL_FRect{ x, 0.0f, frameWidth, textureHeight };
    }

    spriteSet.sizePx = { frameWidth, textureHeight };
    spriteSet.originPx = { frameWidth * originFactor.x, textureHeight * originFactor.y };
    return spriteSet;
}
