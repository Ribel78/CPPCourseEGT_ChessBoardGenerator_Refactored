//TextureFactory.cpp

#include <iostream>
#include "TextureFactory.h"

TextureFactory::TextureFactory(){}

//Cleanup loaded fonts and destroy textures on game exit
TextureFactory::~TextureFactory(){
    //closing fonts
    std::map<std::string, TTF_Font*>::iterator f_it;

    for (f_it = m_fonts.begin(); f_it !=  m_fonts.end(); f_it++)
    {
        TTF_CloseFont(f_it->second);
    }

    //destroying textures
    std::map<std::string, SDL_Texture*>::iterator t_it;

    for(t_it = m_textures.begin(); t_it != m_textures.end(); t_it++)
    {
        SDL_DestroyTexture(t_it->second);
    }
}

void TextureFactory::setRenderer(SDL_Renderer *renderer)
{
    m_renderer = renderer;
}

auto TextureFactory::getRenderer() const -> SDL_Renderer*
{
    return m_renderer;
}

//Produce static textures ttf fonts and store into a map
auto TextureFactory::loadFont(const char* fileName,
                              const std::string& font_id,
                              int font_size) -> bool
{
    if(TTF_Init() == -1)
    {
		return false;
	}
	// loading fonts into pointer variables
	TTF_Font* font = TTF_OpenFont(fileName, font_size); //chess pieces

    if(font == NULL)
    {
		return false;
    } else
    {
        m_fonts[font_id] = font; // add font into the map
        return true;
    }
}

auto TextureFactory::getFont(const std::string& font_id) const -> TTF_Font*
{
    auto it = m_fonts.find(font_id);
    if(it != m_fonts.end())
    {
        return it->second;
    }

    return nullptr;
}

//Produce static textures from images files and store into a map
auto TextureFactory::textureFromImage(const char* fileName,
                                      const std::string& texture_id) -> bool
{

    SDL_Texture* tex = NULL;

    //Init SDL_Image library
    // load support for PNG and JPG image formats
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    //std::cout << "IMG_Init flags " << img_init <<std::endl;

    tex = IMG_LoadTexture(m_renderer, fileName);

    if(tex == NULL)
    {
        std::cout
            << "Cant' load texture from "
            << fileName
            << std::endl;
    } else
    {
        m_textures[texture_id] = tex; //add the created texture in the textureMap map
        IMG_Quit();
        return true;
    }

    return false;
}

/*
Creates a texture from loaded font with optional change of font size
_id - id of the texture to be stored
_font_id - id of existing font
_text - text to be typed in the texture
_fg - color of the text, can be typed as {R ,G ,B ,A}
_wrap_length - length of the box in pixels to start wrapping the text, if 0 wraps only on \n 
_ren - renderer 
int font_size - set the fornt size, if 0 - uses the initial size when font was loaded
*/
bool TextureFactory::textureFromFont(const std::string& texture_id,
                                     const std::string& font_id,
                                     const char* text,
                                     const SDL_Color& fg,
                                     Uint32 wrap_length,
                                     int font_size = 0)
{
	SDL_Surface* tempSurfaceText = NULL;

    TTF_Font* font = m_fonts[font_id];

    tempSurfaceText = TTF_RenderText_Blended_Wrapped(font, text, fg, wrap_length);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, tempSurfaceText);

    SDL_FreeSurface(tempSurfaceText);

    m_textures[texture_id] = tex;
    if(tex)
        return true;
    else
        return false;
}
//_unicode - of type "\u265A"
auto TextureFactory::textureFromUnicode(const std::string& texture_id,
                                         const std::string& font_id,
                                         const char* unicode,
                                         const SDL_Color& fg) -> bool
{
    TTF_Font* font = getFont(font_id);

    SDL_Surface* tempSurfaceText = NULL;

    tempSurfaceText = TTF_RenderUTF8_Blended(font, unicode, fg);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, tempSurfaceText);

    SDL_FreeSurface(tempSurfaceText);

    m_textures[texture_id] = tex;
    if(tex)
        return true;
    else
        return false;
}
//UNUSED
void TextureFactory::packTexture(const std::string& texture_id, const std::string& pack_id)
{
    m_texturePack[pack_id].push_back(m_textures[texture_id]);
}
//UNUSED
auto TextureFactory::getTextureFromPack(const std::string& pack_id, int tex_idx) -> SDL_Texture*
{
    if (tex_idx < m_texturePack[pack_id].size())
        return m_texturePack[pack_id][tex_idx];
    else
        return NULL;
}

void TextureFactory::drawTexture(const std::string& texture_id,
                                 const SDL_Rect* srcrect,
                                 const SDL_Rect* dstrect)
{
    SDL_RenderCopy(m_renderer,
                   m_textures[texture_id],
                   srcrect,
                   dstrect);
}

void TextureFactory::setTextureAlpha(std::string texture_id, Uint8 alpha){
    SDL_SetTextureAlphaMod(m_textures[texture_id], alpha);
    SDL_SetTextureBlendMode(m_textures[texture_id], SDL_BLENDMODE_BLEND);
}

// Destroy a texture by given id
void TextureFactory::destroyTexture(const std::string& texture_id)
{
    SDL_DestroyTexture(m_textures[texture_id]);
}

// Keep single static instance of the Texture Factory class
TextureFactory* TextureFactory::instance(){
    if(m_instance == 0)
    {
        m_instance = new TextureFactory();
        return m_instance;
    }
        return m_instance;
}

// Initialize static instance of Texture Factory object
TextureFactory* TextureFactory::m_instance = 0;
