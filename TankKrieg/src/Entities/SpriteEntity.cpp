#include "Entities/SpriteEntity.h"

#include "Math/IsoUtils.h"

const SpriteRenderData& SpriteEntity::GetSpriteRenderData() const
{
    return sprite;
}

void SpriteEntity::SetSpriteTexture(SDL_Texture* texture)
{
    sprite.texture = texture;
}

void SpriteEntity::SetSpriteSourceRect(const SDL_FRect& sourceRect)
{
    sprite.sourceRect = sourceRect;
    sprite.hasSourceRect = true;
}

void SpriteEntity::ClearSpriteSourceRect()
{
    sprite.sourceRect = { 0.0f, 0.0f, 0.0f, 0.0f };
    sprite.hasSourceRect = false;
}

void SpriteEntity::SetSpriteSizePx(const Vector2& sizePx)
{
    sprite.sizePx = sizePx;
}

void SpriteEntity::SetSpriteOriginPx(const Vector2& originPx)
{
    sprite.originPx = originPx;
}

Vector2 SpriteEntity::GetSpriteAnchorPoint() const
{
    return GetSpriteWorldAnchor();
}

SDL_FRect SpriteEntity::GetSpriteDestinationRect(const RenderContext& ctx) const
{
    const Vector2 worldAnchor = GetSpriteAnchorPoint();
    const SDL_FPoint screenAnchor = IsoUtils::GridToScreenF(
        worldAnchor.x,
        worldAnchor.y,
        ctx.tileWidthPx,
        ctx.tileHeightPx,
        ctx.originXPx,
        ctx.originYPx);

    return SDL_FRect{
        screenAnchor.x - sprite.originPx.x,
        screenAnchor.y - sprite.originPx.y,
        sprite.sizePx.x,
        sprite.sizePx.y
    };
}

Vector2 SpriteEntity::GetSpriteWorldAnchor() const
{
    // Sprite entities render from the same base point used for world sorting.
    return GetRenderSortPoint();
}

void SpriteEntity::RenderSprite(const RenderContext& ctx) const
{
    if (sprite.texture == nullptr || sprite.sizePx.x <= 0.0f || sprite.sizePx.y <= 0.0f)
    {
        return;
    }

    const SDL_FRect destinationRect = GetSpriteDestinationRect(ctx);
    const SDL_FRect* sourceRect = sprite.hasSourceRect ? &sprite.sourceRect : nullptr;

    SDL_RenderTexture(ctx.renderer, sprite.texture, sourceRect, &destinationRect);
}
