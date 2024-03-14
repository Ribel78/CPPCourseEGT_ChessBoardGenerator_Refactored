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

    // delete static instance of Texture Factory
    //delete m_instance;
}

void TextureFactory::setRenderer(SDL_Renderer *renderer)
{
    m_renderer = renderer;
}

SDL_Renderer *TextureFactory::getRenderer()
{
    return m_renderer;
}

//Produce static textures ttf fonts and store into a map
bool TextureFactory::loadFont(const char* fileName,
                              const std::string& id,
                              int font_size)
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
        m_fonts[id] = font; // add font into the map
        return true;
    }
}

TTF_Font* TextureFactory::getFont(const std::string& font_id) const
{
    auto it = m_fonts.find(font_id);
    if(it != m_fonts.end())
    {
        return it->second;
    }

    return nullptr;
}

//Produce static textures from images files and store into a map
bool TextureFactory::textureFromImage(const char* fileName,
                                      const std::string& id){

    SDL_Texture* tex = IMG_LoadTexture(m_renderer, fileName);

    if(tex == NULL)
    {
        std::cout
            << "Cant' load texture from "
            << fileName
            << std::endl;
    } else
    {
        m_textures[id] = tex; //add the created texture in the textureMap map
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
bool TextureFactory::textureFromFont(const std::string& id,
                                     const std::string& font_id,
                                     const char* text,
                                     const SDL_Color& fg,
                                     Uint32 wrap_length,
                                     int font_size = 0)
{
    //create temp surface
	SDL_Surface* tempSurfaceText = NULL;
    //choose a font
    TTF_Font* font = m_fonts[font_id];

    tempSurfaceText = TTF_RenderText_Blended_Wrapped(font,
                                                     text,
                                                     fg,
                                                     wrap_length);
    //create a texture
    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer,
                                                    tempSurfaceText);

    //free temp surface
    SDL_FreeSurface(tempSurfaceText);

    m_textures[id] = tex;
    if(tex)
    {
        return true;
    } else
    {
        return false;
    } 
}

void TextureFactory::drawTexture(const std::string& tex_id,
                                 const SDL_Rect* srcrect,
                                 const SDL_Rect* dstrect)
{
    SDL_RenderCopy(m_renderer,
                   m_textures[tex_id],
                   NULL,
                   dstrect);
}

//individual call to destroy a texture by id usually used in the dynamic elements
void TextureFactory::destroyTexture(const std::string& tex_id)
{
    SDL_DestroyTexture(m_textures[tex_id]);
}

//call static instance of the Texture Factory
TextureFactory* TextureFactory::instance(){
    if(m_instance == 0)
    {
        m_instance = new TextureFactory();
        return m_instance;
    }
        return m_instance;
}

//initialize static instance of Texture Factory object
TextureFactory* TextureFactory::m_instance = 0;
