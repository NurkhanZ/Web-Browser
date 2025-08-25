#include "SmartRenderer.h"

SmartRenderer::SmartRenderer(SDL_Window* g_window){
    SDL_Renderer* Ptr = SDL_CreateRenderer(
    g_window, 
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    w_renderer = static_cast<UniqueSDLRenderer>(Ptr);
}

SDL_Renderer* SmartRenderer::GetRaw() const {
    return w_renderer.get();
}