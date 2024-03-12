#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>
#include <string>

class TextureFactory
{

public:
    TextureFactory();
    ~TextureFactory();

    bool loadFont(const char* fileName,
                  std::string id,
                  int font_size);

    TTF_Font* getFont(std::string font_id) const;

    bool textureFromImage(const char* fileName,
                          std::string id,
                          SDL_Renderer* ren);

    bool textureFromFont(std::string id,
                          std::string font_id,
                          const char* text,
                          SDL_Color fg,
                          Uint32 wrap_length,
                          SDL_Renderer* ren,
                          int font_size);

    void drawTexture(SDL_Renderer* ren,
                     std::string tex_id,
                     SDL_Rect* srcrect,
                     SDL_Rect* dstrect);

    void destroyTexture(std::string tex_id);

    static TextureFactory* instance();



private:

    static TextureFactory* m_instance;
    std::map<std::string, TTF_Font*> m_fonts;
    std::map<std::string, SDL_Texture*> m_textures;
};
