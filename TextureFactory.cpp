#include "TextureFactory.h"

TextureFactory::TextureFactory(){}

//Cleanup loaded fonts and destroy textures on game exit
TextureFactory::~TextureFactory(){
    //closing fonts
    std::map<std::string, TTF_Font*>::iterator f_it;
    for (f_it = fonts.begin(); f_it !=  fonts.end(); f_it++){
        TTF_CloseFont(f_it->second);
    }
    //destroying textures
    std::map<std::string, SDL_Texture*>::iterator t_it;
    for(t_it = textures.begin(); t_it != textures.end(); t_it++){
        SDL_DestroyTexture(t_it->second);
    }
    // delete static instance of Texture Factory
    delete instance;
}

//Produce static textures ttf fonts and store into a map
bool TextureFactory::loadFont(const char* fileName, std::string id, int font_size){
	if(TTF_Init() == -1){
		return false;
	}
	// loading fonts into pointer variables
	TTF_Font* font = TTF_OpenFont(fileName, font_size); //chess pieces

	if(font == NULL){
		return false;
	} else {
        fonts[id] = font; // add font into the map
        return true;
    }
}
//Produce static textures from images files and store into a map
bool TextureFactory::textureFromImage(const char* fileName, std::string id, SDL_Renderer* ren){

    SDL_Texture* tex = IMG_LoadTexture(ren, fileName);
    if(tex == NULL){
        std::cout<< "Cant' load texture from " << fileName << std::endl;
    } else {
        textures[id] = tex; //add the created texture in the textureMap map
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
bool TextureFactory::textureFromFont(   std::string id, 
                                        std::string font_id, 
                                        const char* text, SDL_Color fg,
                                        Uint32 wrap_length, 
                                        SDL_Renderer* ren,
                                        int font_size = 0){
    //creating temp surface
	SDL_Surface* tempSurfaceText = NULL;

    TTF_Font* font = fonts[font_id];

    tempSurfaceText = TTF_RenderText_Blended_Wrapped(font, text, fg, wrap_length);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurfaceText);

    //free temporaary surface
    SDL_FreeSurface(tempSurfaceText);

    textures[id] = tex;
    if(tex){
        return true;
    } else {
        return false;
    }
    
    
}

void TextureFactory::drawTexture(   SDL_Renderer* ren, 
                                    std::string tex_id, 
                                    SDL_Rect* srcrect, 
                                    SDL_Rect* dstrect){

	//Use SDL_QueryTexture to get textures dimensions;
	//TODO  add some extra cropping functionality
	SDL_RenderCopy(ren, textures[tex_id], NULL, dstrect);	   
}

//Individual call to destroy a texture by id usually used in the dynamic elements
void TextureFactory::destroyTexture(std::string tex_id){
    
    SDL_DestroyTexture(textures[tex_id]);

}

//Call static instance of the Texture Factory
TextureFactory* TextureFactory::Instance(){
    if(instance == 0){
        instance = new TextureFactory();
        return instance;
    }
        return instance;
}

// initialize static instance of Texture Factory object
TextureFactory* TextureFactory::instance = 0;
