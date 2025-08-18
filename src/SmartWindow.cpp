#include "SmartWindow.h"

SmartWindow::SmartWindow(){
    SDL_Window* Ptr = SDL_CreateWindow(
        "Hello Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MINIMIZED
    );

    w_window = static_cast<UniqueSDLWindow>(Ptr);

    // SDL_PixelFormat* PixelFormat = GetSurface()->format;
    // Uint32 RedColor = SDL_MapRGB(PixelFormat, 255, 0, 0);

    // SDL_FillRect(
    //     GetSurface(),
    //     nullptr,
    //     RedColor
    // );
}

SDL_Window* SmartWindow::GetRaw() const {
    return w_window.get();
}

SDL_Surface* SmartWindow::GetSurface() const {
    return SDL_GetWindowSurface(GetRaw());
}

void SmartWindow::render(){
    SDL_UpdateWindowSurface(GetRaw());
}