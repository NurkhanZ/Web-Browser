#ifndef RENDERER
#define RENDERER

#pragma once
#include "TagSystem.h"

class Renderer{
    public:
        Renderer(Root* root, string title);
        void processTree();
        void drawTree();
        void maintainWindow();

        // sf::RenderWindow window; SDL_WIndow
        // sf::Font font; FontManager 
    private:
        Root* root;
};

#endif