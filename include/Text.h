#pragma once

#include "GenericTag.h"

class Text : public GenericTag{
    public:
        Text();
        void specificProccess(ProccessRect& rect);
        void specificDraw(DrawRect& rect);
    private:
        int font_size;
        // sf::Text text; adapt to sdl_ttf;
        int GetTextStyle();
};