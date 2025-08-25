#include "SmartFont.h"

SmartFont::SmartFont(const char* content) : g_font {TTF_OpenFont("Arial.ttf", 50)} {}

void SmartFont::render(SDL_Surface* destination_surface){

}

TTF_Font* SmartFont::GetRaw() const{
    return g_font.get();
}; 