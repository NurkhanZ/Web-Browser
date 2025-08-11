#include "SmartWindow.hpp"

Window::Window(){
    SDL_Window* Ptr = SDL_CreateWindow(
        "Hello Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        700, 300,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MINIMIZED
    );

    // Store in the smart pointer
    SDLWindow = static_cast<UniqueSDLWindow>(Ptr);

    // Get the window surface's pixel format
    SDL_PixelFormat* PixelFormat = GetSurface()->format;

    Uint32 RedColor = SDL_MapRGB(PixelFormat, 255, 0, 0);

    Uint32 GreenColor{SDL_MapRGB(PixelFormat, 0, 255, 0)};
    Uint32 BlueColor{SDL_MapRGB(PixelFormat, 0, 0, 255)};

    Uint32 Color{SDL_MapRGBA(PixelFormat, 255, 0, 0, 200)};

    SDL_FillRect(
        GetSurface(),
        nullptr,
        RedColor
    ); 
}

// A way to get raw pointer accessible for SDL functions
SDL_Window* Window::GetRaw() const {
    return SDLWindow.get();
}

SDL_Surface* Window::GetSurface() const {
    // do not store window surface pointer, as it can will become dangling pointer after window is resized
    return SDL_GetWindowSurface(GetRaw());
}

void Window::render(){
    SDL_UpdateWindowSurface(GetRaw());
}
