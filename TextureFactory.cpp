//TextureFactory.cpp

#include <iostream>
#include "TextureFactory.h"

TextureFactory::TextureFactory(){}

//Cleanup loaded fonts and destroy textures on game exit
TextureFactory::~TextureFactory(){
    //closing fonts
    std::map<std::string, TTF_Font*>::iterator fontIt;

    for (fontIt = m_fonts.begin(); fontIt !=  m_fonts.end(); fontIt++)
    {
        TTF_CloseFont(fontIt->second);
    }

    //destroying textures
    std::map<std::string, SDL_Texture*>::iterator textureIt;

    for(textureIt = m_textures.begin(); textureIt != m_textures.end(); textureIt++)
    {
        SDL_DestroyTexture(textureIt->second);
    }
}

void TextureFactory::SetRenderer(SDL_Renderer *renderer)
{
    m_renderer = renderer;
}

auto TextureFactory::GetRenderer() const -> SDL_Renderer*
{
    return m_renderer;
}

//Produce static textures ttf fonts and store into a map
auto TextureFactory::LoadFont(const char* fileName,
                              const std::string& fontId,
                              int fontSize) -> bool
{
    if(TTF_Init() == -1)
    {
		return false;
	}

    TTF_Font* font = TTF_OpenFont(fileName, fontSize);

    if(font == NULL)
    {
		return false;
    } else
    {
        m_fonts[fontId] = font;
        return true;
    }
}

auto TextureFactory::GetFont(const std::string& fontId) const -> TTF_Font*
{
    auto it = m_fonts.find(fontId);
    if(it != m_fonts.end())
    {
        return it->second;
    }

    return nullptr;
}

//Produce static textures from images files and store into a map
auto TextureFactory::TextureFromImage(const char* fileName,
                                      const std::string& textureId) -> bool
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
        m_textures[textureId] = tex; //add the created texture in the textureMap map
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
bool TextureFactory::TextureFromFont(const std::string& textureId,
                                     const std::string& fontId,
                                     const char* text,
                                     const SDL_Color& fg,
                                     Uint32 wrapLength,
                                     int fontSize = 0)
{
	SDL_Surface* tempSurfaceText = NULL;

    TTF_Font* font = m_fonts[fontId];

    tempSurfaceText = TTF_RenderText_Blended_Wrapped(font, text, fg, wrapLength);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, tempSurfaceText);

    SDL_FreeSurface(tempSurfaceText);

    m_textures[textureId] = tex;
    if(tex)
        return true;
    else
        return false;
}
//_unicode - of type "\u265A"
auto TextureFactory::TextureFromUnicode(const std::string& textureId,
                                         const std::string& fontId,
                                         const char* unicode,
                                         const SDL_Color& fg) -> bool
{
    TTF_Font* font = GetFont(fontId);

    SDL_Surface* tempSurfaceText = NULL;

    tempSurfaceText = TTF_RenderUTF8_Blended(font, unicode, fg);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, tempSurfaceText);

    SDL_FreeSurface(tempSurfaceText);

    m_textures[textureId] = tex;
    if(tex)
        return true;
    else
        return false;
}
//UNUSED
void TextureFactory::PackTexture(const std::string& textureId, const std::string& packId)
{
    m_texturePack[packId].push_back(m_textures[textureId]);
}
//UNUSED
auto TextureFactory::GetTextureFromPack(const std::string& packId, int texIdx) -> SDL_Texture*
{
    if (texIdx < m_texturePack[packId].size())
        return m_texturePack[packId][texIdx];
    else
        return NULL;
}

void TextureFactory::DrawTexture(const std::string& textureId,
                                 const SDL_Rect* srcRect,
                                 const SDL_Rect* dstRect)
{
    SDL_RenderCopy(m_renderer,
                   m_textures[textureId],
                   srcRect,
                   dstRect);
}

void TextureFactory::SetTextureAlpha(std::string textureId, Uint8 alpha){
    SDL_SetTextureAlphaMod(m_textures[textureId], alpha);
    SDL_SetTextureBlendMode(m_textures[textureId], SDL_BLENDMODE_BLEND);
}

// Destroy a texture by given id
void TextureFactory::DestroyTexture(const std::string& textureId)
{
    SDL_DestroyTexture(m_textures[textureId]);
}

// Keep single static Instance of the Texture Factory class
TextureFactory* TextureFactory::Instance(){
    if(m_instance == 0)
    {
        m_instance = new TextureFactory();
        return m_instance;
    }
        return m_instance;
}

// Initialize static instance of Texture Factory object
TextureFactory* TextureFactory::m_instance = 0;
