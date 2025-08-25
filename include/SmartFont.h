#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>

struct UniqueFontDeleter{
    void operator()(TTF_Font* Ptr) const {
        if(Ptr && TTF_WasInit()){
            TTF_CloseFont(Ptr);
            TTF_Quit();
        }
    }
};

using UniqueFont = std::unique_ptr<TTF_Font, UniqueFontDeleter>;


// Write prototypes for font functions (like setCharacterSize)
class SmartFont{
    public:
        SmartFont(const char* content);
        void render(SDL_Surface* destination_surface);

        TTF_Font* GetRaw() const; 
    private:
        UniqueFont g_font = nullptr;
};