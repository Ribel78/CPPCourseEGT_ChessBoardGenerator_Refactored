#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <vector>

class TextureFactory
{

public:
    TextureFactory();
    ~TextureFactory();

    void SetRenderer(SDL_Renderer* renderer);

    auto GetRenderer() const -> SDL_Renderer*;

    auto LoadFont(const char* fileName,
                  const std::string& fontId,
                  int fontSize) -> bool;

    auto GetFont(const std::string& fontId) const -> TTF_Font*;

    auto TextureFromImage(const char* fileName,
                          const std::string& textureId) -> bool;

    auto TextureFromFont(const std::string& textureId,
                         const std::string& fontId,
                         const char* text,
                         const SDL_Color& fg,
                         Uint32 wrapLength,
                         int fontSize) -> bool;

    auto TextureFromUnicode(const std::string& textureId,
                         const std::string& fontId,
                         const char* unicode,
                         const SDL_Color& fg) -> bool;

    void PackTexture(const std::string& textureId, const std::string& packId);

    auto GetTextureFromPack(const std::string& packId, int texIdx) -> SDL_Texture*;

    void DrawTexture(const std::string& textureId,
                     const SDL_Rect* srcRect,
                     const SDL_Rect* dstRect);

    void SetTextureAlpha(std::string textureId, Uint8 alpha);

    void DestroyTexture(const std::string& textureId);

    static TextureFactory* Instance();

private:

    SDL_Renderer* m_renderer = nullptr;
    static TextureFactory* m_instance;
    std::map<std::string, TTF_Font*> m_fonts;
    std::map<std::string, SDL_Texture*> m_textures;
    std::map<std::string, std::vector<SDL_Texture*>> m_texturePack;
};
