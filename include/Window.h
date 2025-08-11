#pragma once
#include <SDL.h>

class Window {
public:
  Window(){
    SDLWindow = SDL_CreateWindow(
      "Hello Window",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      700, 300, 0
    );

    SDL_FillRect(
      GetSurface(),
      nullptr,
      SDL_MapRGB(
        GetSurface()->format, 50, 50, 50
      )
    );

    SDL_UpdateWindowSurface(SDLWindow);
  }

  SDL_Surface* GetSurface() const {
    return SDL_GetWindowSurface(SDLWindow);
  }
  
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  
  ~Window() {
    if (SDLWindow && SDL_WasInit(SDL_INIT_VIDEO)) {
      SDL_DestroyWindow(SDLWindow);
    }
  }
  
private:
  SDL_Window* SDLWindow{nullptr};
};