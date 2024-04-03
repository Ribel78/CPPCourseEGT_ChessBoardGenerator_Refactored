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

    void setRenderer(SDL_Renderer* renderer);

    auto getRenderer() const -> SDL_Renderer*;

    auto loadFont(const char* fileName,
                  const std::string& font_id,
                  int font_size) -> bool;

    auto getFont(const std::string& font_id) const -> TTF_Font*;

    auto textureFromImage(const char* fileName,
                          const std::string& texture_id) -> bool;

    auto textureFromFont(const std::string& texture_id,
                         const std::string& font_id,
                         const char* text,
                         const SDL_Color& fg,
                         Uint32 wrap_length,
                         int font_size) -> bool;

    auto textureFromUnicode(const std::string& texture_id,
                         const std::string& font_id,
                         const char* unicode,
                         const SDL_Color& fg) -> bool;

    void packTexture(const std::string& texture_id, const std::string& pack_id);

    auto getTextureFromPack(const std::string& pack_id, int tex_idx) -> SDL_Texture*;

    void drawTexture(const std::string& texture_id,
                     const SDL_Rect* srcrect,
                     const SDL_Rect* dstrect);

    void setTextureAlpha(std::string texture_id, Uint8 alpha);

    void destroyTexture(const std::string& texture_id);

    void loadResources();

    static TextureFactory* instance();

private:

    SDL_Renderer* m_renderer = NULL;
    static TextureFactory* m_instance;
    std::map<std::string, TTF_Font*> m_fonts;
    std::map<std::string, SDL_Texture*> m_textures;
    std::map<std::string, std::vector<SDL_Texture*>> m_texturePack;
};
