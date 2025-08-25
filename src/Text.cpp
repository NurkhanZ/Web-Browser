#include "Text.h"

Text::Text(){
    this->type = TagType::TEXT;
    this->standalone = true;
    this->wrapping = false;
}

void Text::specificProccess(ProccessRect& rect){
    this->font_size = safe_stoi(this->getAttribute(FONTSIZE));
    // Make prototypes for these functions in SmartFont.h 
    // setFont
    // setString
    // setCharacterSize
    // setStyle
    // this->bounding_box.x = this->text.getLocalBounds.width() + this.text.getLocalBounds().left + 2;
    this->bounding_box.y = this->font_size * 115 /100;
}

void Text::specificDraw(DrawRect& rect){
    // setColor
    // setPosition
    // setOrigin
    // draw text
}

int Text::GetTextStyle(){
    int x = 0;
    if(!(this->getAttribute(BOLD).empty())){
        // x |= setTextBold
    }
    else if(!(this->getAttribute(ITALIC).empty())){
        // x |= setTextItalic
    }
    if(!(this->getAttribute(UNDERLINED).empty())){
        // x |= setTextUnderlined
    }
    return x;
}