#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

class TextureFactory
{

public:
    TextureFactory();
    ~TextureFactory();

    void setRenderer(SDL_Renderer* renderer);

    SDL_Renderer* getRenderer();

    bool loadFont(const char* fileName,
                  const std::string& font_id,
                  int font_size);

    TTF_Font* getFont(const std::string& font_id) const;

    bool textureFromImage(const char* fileName,
                          const std::string& texture_id);

    bool textureFromFont(const std::string& texture_id,
                         const std::string& font_id,
                         const char* text,
                         const SDL_Color& fg,
                         Uint32 wrap_length,
                         int font_size);

    void drawTexture(const std::string& texture_id,
                     const SDL_Rect* srcrect,
                     const SDL_Rect* dstrect);

    void setTextureAlpha(std::string texture_id, Uint8 alpha);

    void destroyTexture(const std::string& texture_id);

    static TextureFactory* instance();

private:

    SDL_Renderer* m_renderer = NULL;
    static TextureFactory* m_instance;
    std::map<std::string, TTF_Font*> m_fonts;
    std::map<std::string, SDL_Texture*> m_textures;
};
