#include <iostream>
#include <SDL.h>
// #include "Window.h"
#include "SmartWindow.h"
#include "RenderQueue.h"

#define DEBUG

using namespace std;

int initWindow();
int initRenderer();
int initFont();
void close();
void checkEvents(SDL_Event event, bool& running);

SDL_Window* g_window = SDL_CreateWindow(
    "Web Browser",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    1920, 1080,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

SDL_Renderer* g_renderer = SDL_CreateRenderer(
    g_window, 
    -1,
     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


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

    std::ifstream htmlFile(htmlPath);

    std::string html;

    if(htmlFile.is_open()){
        std::string line;
        while(std::getline(htmlFile, line)){
            html += line;
        }
        htmlFile.close();
    }
    else{
        std::cerr << "Unable to open file: " << htmlPath << std::endl;
        close();
        return EXIT_FAILURE;
    }
    
    HTMLDocument doc(html);
    
    SmartWindow w_window;

    SDL_Event event;
    bool running = true;

    while(running){
        // ..
        checkEvents(event, running);
        SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
        SDL_RenderClear(g_renderer);

        rq.populateQueue(doc.getRoot());
        rq.render();

        SDL_RenderPresent(g_renderer);
        // w_window.render();
    }
    close();
    return EXIT_SUCCESS;
}

int initWindow(){
    #ifdef DEBUG
        std::cout << "Initializing Window" << std::endl;
    #endif

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << "SDL couldn't init window" << std:: endl;
        return EXIT_FAILURE;
    }

    if(!g_window){
        std::cerr << "Window couldn't be created" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int initRenderer(){
    #ifdef DEBUG
        std::cout << "Initializing Renderer" << std::endl;
    #endif

    if(!g_renderer){
        std::cerr << "Renderer couldn't be created" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int initFont(){
    #ifdef DEBUG
        std::cout << "Initializing TTF" << std::endl;
    #endif

    if(TTF_Init() < 0){
        std::cerr << "SDL_ttf could't initialize: " << TTF_GetError() << std::endl;
        std::cerr << SDL_GetError();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void checkEvents(SDL_Event event, bool& running){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            running = false;
        }
        if(event.type == SDL_WINDOWEVENT){
            switch (event.window.event){
                case SDL_WINDOWEVENT_RESIZED:
                    break;
            }
        }
    }
}

void close(){
    TTF_Quit();
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}