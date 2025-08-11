#include <iostream>
#include <SDL.h>
// #include "Window.h"
#include "SmartWindow.h"

using namespace std;

void close();

void checkEvents(SDL_Event event, bool& running);

int main(int argc, char* argv[]){

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cerr << "SDL_Error: " << SDL_GetError() << endl;  
    }

    SmartWindow w_window;

    SDL_Event event;
    bool running = true;

    while(running){
        // ..
        checkEvents(event, running);
        w_window.render();
    }
    close();
    return 0;
}

void checkEvents(SDL_Event event, bool& running){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            running = false;
        }
    }
}

void close(){
    SDL_Quit();
}