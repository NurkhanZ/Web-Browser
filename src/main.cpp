#include <iostream>
#include <SDL.h>
// #include "Window.h"
#include "SmartWindow.h"
#include "SmartRenderer.h"
#include "RenderQueue.h"

#define DEBUG

using namespace std;

int initWindow();
int initRenderer();
int initFont();
void close();
void checkEvents(SDL_Event event, bool& running);

SmartWindow g_window;
SDL_Window* pointer_window = g_window.GetRaw();

SmartRenderer w_renderer(pointer_window);
SDL_Renderer* g_renderer = w_renderer.GetRaw();

int main(int argc, char* argv[]){
    if(initWindow() == EXIT_FAILURE ||
       initRenderer() == EXIT_FAILURE ||
       initFont() == EXIT_FAILURE){
        close();
        return EXIT_FAILURE;
       }

    std::string htmlPath = "../html/test.html";
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
    
    SDL_Event event;
    bool running = true;
    
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_renderer);

    while(running){
        checkEvents(event, running);
        
        rq.populateQueue(doc.getRoot());
        rq.render();

        SDL_RenderPresent(g_renderer);
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

    if(!g_window.GetRaw()){
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
        std::cout << "SDL_Error " << SDL_GetError() << std::endl;
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
    SDL_Quit();
}