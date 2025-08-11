#include <iostream>
#include "globals.h"
#include "SmartWindow.hpp"
// #include "Window.h"


void checkEvents(SDL_Event event, bool& running);
void close();

int main(int argc, char* argv[]){
    SDL_Init(0);

    Window GameWindow;

    std::string htmlPath = "../html/index.html";

    if(argc > 1){
        htmlPath = argv[1];
    }

    SDL_Event event;
    bool running = true;

    while(running){
        checkEvents(event, running);
        
        GameWindow.render();
    }

    close();
    return EXIT_SUCCESS;
}

void checkEvents(SDL_Event event, bool& running){
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT){
            running = false;
        }
    }
}

void close(){
    SDL_Quit();
}