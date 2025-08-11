#include <iostream>
#include <SDL.h>
#include "Window.h"

using namespace std;

void close();

int main(int argc, char* argv[]){

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cerr << "SDL_Error: " << SDL_GetError() << endl;  
    }

    Window g_window;

    SDL_Event event;
    bool running = true;

    while(running){
        // main loopp

        SDL_Quit();
    }

    return 0;
}

void close(){
    SDL_Quit();
}