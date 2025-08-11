#pragma once
#include "globals.h"
#include <memory>

struct SDLWindowDeleter{
    void operator()(SDL_Window* Ptr) const {
        if(Ptr && SDL_WasInit(SDL_INIT_VIDEO)){
            SDL_DestroyWindow(Ptr);
        }
    }
};

using UniqueSDLWindow = std::unique_ptr<SDL_Window, SDLWindowDeleter>;

class SmartWindow{
    public:
        SmartWindow();

        SDL_Window* GetRaw() const;

        SDL_Surface* GetSurface() const;

        void render();
    private:
        UniqueSDLWindow w_window = nullptr;
};
