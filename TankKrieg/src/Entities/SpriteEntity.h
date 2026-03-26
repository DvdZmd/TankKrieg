#pragma once

#include <SDL3/SDL.h>

#include "Entities/Entity.h"
#include "Render/RenderContext.h"

struct SpriteRenderData
{
    SDL_Texture* texture = nullptr;
    SDL_FRect sourceRect{ 0.0f, 0.0f, 0.0f, 0.0f };
    Vector2 sizePx{ 0.0f, 0.0f };
    Vector2 originPx{ 0.0f, 0.0f };
    bool hasSourceRect = false;
};

class SpriteEntity : public Entity
{
public:
    const SpriteRenderData& GetSpriteRenderData() const;

    void SetSpriteTexture(SDL_Texture* texture);
    void SetSpriteSourceRect(const SDL_FRect& sourceRect);
    void ClearSpriteSourceRect();
    void SetSpriteSizePx(const Vector2& sizePx);
    void SetSpriteOriginPx(const Vector2& originPx);

    Vector2 GetSpriteAnchorPoint() const;
    SDL_FRect GetSpriteDestinationRect(const RenderContext& ctx) const;

protected:
    virtual Vector2 GetSpriteWorldAnchor() const;
    void RenderSprite(const RenderContext& ctx) const;

private:
    SpriteRenderData sprite{};
};
