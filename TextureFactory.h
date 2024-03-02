#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>
#include <string>

class TextureFactory{

public:
TextureFactory();
~TextureFactory();
bool loadFont(const char*, std::string, int);
bool textureFromImage(const char*, std::string, SDL_Renderer*);
bool textureFromFont(std::string, std::string, const char*, SDL_Color, Uint32 wrap_length, SDL_Renderer*, int);
void drawTexture(SDL_Renderer*, std::string, SDL_Rect* , SDL_Rect*);

static TextureFactory* Instance();
std:: map <std::string, TTF_Font*> fonts;
private:

static TextureFactory* instance;
std:: map <std::string, SDL_Texture*> textures;


};