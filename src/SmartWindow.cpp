#include "SmartWindow.h"

SmartWindow::SmartWindow(){
    SDL_Window* Ptr = SDL_CreateWindow(
        "Hello Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        getWidth(),
        getHeight(),
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MINIMIZED
    );

    // Store in the smart pointer
    SDLWindow = static_cast<UniqueSDLWindow>(Ptr);

}

// A way to get raw pointer accessible for SDL functions
SDL_Window* SmartWindow::GetRaw() const {
    return SDLWindow.get();
}

SDL_Surface* SmartWindow::GetSurface() const {
    // do not store window surface pointer, as it can will become dangling pointer after window is resized
    return SDL_GetWindowSurface(GetRaw());
}

void SmartWindow::render(){
    // Get the window surface's pixel format
    SDL_PixelFormat* PixelFormat = GetSurface()->format;

    Uint32 RedColor = SDL_MapRGB(PixelFormat, 255, 0, 0);

    SDL_FillRect(
        GetSurface(),
        nullptr,
        RedColor
    );     
}

void SmartWindow::update(){
    SDL_UpdateWindowSurface(GetRaw());
}

int SmartWindow::getWidth() const { return 800; }

int SmartWindow::getHeight() const { return 600; }