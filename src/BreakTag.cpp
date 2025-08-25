#include "BreakTag.h"

BreakTag::BreakTag(){
    this->type = TagType::BR;
    this->standalone = true;
    this->wrapping = false;
}

void BreakTag::specificProccess(ProccessRect& rect){
    rect.accumulator.y += rect.max_height;
    rect.accumulator.x = 0;
    rect.max_height = 0;
    // sf::Text text;
    int font_size = safe_stoi(this->getAttribute(FONTSIZE));
    // setFont
    // setString
    // setCharacterSize
    this->bounding_box.x = 0;
    // this->bounding_box.y = text.getLocalBounds().height + 2 + font_size * 7 / 100;
}

void BreakTag::specificDraw(DrawRect& rect) {}