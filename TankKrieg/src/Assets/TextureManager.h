#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

class TextureManager
{
public:
    explicit TextureManager(SDL_Renderer* renderer = nullptr);
    ~TextureManager();

    void SetRenderer(SDL_Renderer* renderer);

    SDL_Texture* Load(const std::string& key, const std::string& path);
    SDL_Texture* Find(const std::string& key) const;

    void Clear();

private:
    struct TextureEntry
    {
        SDL_Texture* texture = nullptr;
        std::string path;
    };

    SDL_Renderer* renderer = nullptr;
    std::unordered_map<std::string, TextureEntry> textures;
};
