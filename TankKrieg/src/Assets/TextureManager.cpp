#include "Assets/TextureManager.h"

TextureManager::TextureManager(SDL_Renderer* renderer)
    : renderer(renderer)
{
}

TextureManager::~TextureManager()
{
    Clear();
}

void TextureManager::SetRenderer(SDL_Renderer* value)
{
    renderer = value;
}

SDL_Texture* TextureManager::Load(const std::string& key, const std::string& path)
{
    if (const auto found = textures.find(key); found != textures.end())
    {
        return found->second.texture;
    }

    if (renderer == nullptr)
    {
        SDL_Log("TextureManager has no renderer. Failed to load '%s' from '%s'.", key.c_str(), path.c_str());
        return nullptr;
    }

    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (surface == nullptr)
    {
        SDL_Log("TextureManager could not load '%s' from '%s': %s", key.c_str(), path.c_str(), SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (texture == nullptr)
    {
        SDL_Log("TextureManager could not create texture '%s' from '%s': %s", key.c_str(), path.c_str(), SDL_GetError());
        return nullptr;
    }

    textures.emplace(key, TextureEntry{ texture, path });
    return texture;
}

SDL_Texture* TextureManager::Find(const std::string& key) const
{
    if (const auto found = textures.find(key); found != textures.end())
    {
        return found->second.texture;
    }

    return nullptr;
}

void TextureManager::Clear()
{
    for (auto& [key, entry] : textures)
    {
        if (entry.texture != nullptr)
        {
            SDL_DestroyTexture(entry.texture);
            entry.texture = nullptr;
        }
    }

    textures.clear();
}
