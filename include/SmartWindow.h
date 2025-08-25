#pragma once
#include <SDL.h>
#include <memory>
#include <iostream>


// Define custom deleter for SDL_Window*
struct SDLWindowDeleter{
    void operator()(SDL_Window* Ptr) const{
        if(Ptr && SDL_WasInit(SDL_INIT_VIDEO)){
            SDL_DestroyWindow(Ptr);
        }
    }
};

// Alias for std::unique_ptr
using UniqueSDLWindow = std::unique_ptr<SDL_Window, SDLWindowDeleter>;


class SmartWindow{
    public:
        SmartWindow();    
    
        // A way to get raw pointer accessible for SDL functions
        SDL_Window* GetRaw() const;

        SDL_Surface* GetSurface() const;

        void render();

        void update();

        int getWidth() const;

        int getHeight() const;
        
    private:
        // Store window resource in the std::unique_ptr
        
        UniqueSDLWindow SDLWindow = nullptr;
};