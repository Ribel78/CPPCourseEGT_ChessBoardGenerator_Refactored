#include "TextureFactory.h"


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