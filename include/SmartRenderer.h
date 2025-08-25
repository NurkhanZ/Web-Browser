#pragma once
#include <SDL.h>
#include <memory>

struct SDLRendererDeleter{
    void operator()(SDL_Renderer* Ptr) const {
        if(Ptr && SDL_WasInit(SDL_INIT_VIDEO)){
            SDL_DestroyRenderer(Ptr);
        }
    }
};

using UniqueSDLRenderer = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>;

class SmartRenderer{
    public:
        SmartRenderer(SDL_Window* g_window);

        SDL_Renderer* GetRaw() const;

    private:
        UniqueSDLRenderer w_renderer = nullptr;
};
