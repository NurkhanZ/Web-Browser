#include <iostream>
#include <SDL.h>
// #include "Window.h"
#include "SmartWindow.h"
#include "RenderQueue.h"

using namespace std;

int initWindow();
int initRenderer();
int initFont();
void close();
void checkEvents(SDL_Event event, bool& running);

int main(int argc, char* argv[]){
    if(initWindow() == EXIT_FAILURE ||
       initRenderer() == EXIT_FAILURE ||
       initFont() == EXIT_FAILURE){
        close();
        return EXIT_FAILURE;
       }

    std::string htmlPath = "../html/index.html";
    if(argc > 1){
        htmlPath = argv[1];
    }

    RenderQueue& rq = RenderQueue::getInstance();
    
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