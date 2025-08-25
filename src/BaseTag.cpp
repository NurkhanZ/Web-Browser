#include "BaseTag.h"

BaseTag::BaseTag(){
    this->index = 0;
    this->standalone = false;
    this->block_level = false;
    this->wrapping = true;
    this->parent = nullptr;
}

BaseTag* BaseTag::addChild(BaseTag* child){
    if(child != nullptr){
        this->children.push_back(child);
        child->parent = this;
        child->index = this->children.size() - 1;
        return child;
    }
    return nullptr;
}

TagType BaseTag::stringToType(string type){
    if (type == "br") return BR;
	else if (type == "b") return B;
	else if (type == "i") return I;
	else if (type == "body") return BODY;
	else if (type == "p") return P;
	else if (type == "h1") return H1;
	else if (type == "h2") return H2;
	else if (type == "h3") return H3;
	else if (type == "h4") return H4;
	else if (type == "h5") return H5;
	else if (type == "h6") return H6;
    else return UNDEFINED;
}

BaseTag* BaseTag::copy(){
    BaseTag* new_tag = this->specificCopy();
    new_tag->attributes = this->attributes; //aren't attributes copied already?
    return new_tag;
}

BaseTag* BaseTag::specificCopy(){
	return new BaseTag;
}

string BaseTag::getAttribute(AttributeType type){
    try{
        return this->attributes.at(type);
    }
    catch(...){
        return "";
    }
}

int BaseTag::safe_stoi(string str, size_t* index, int base){
    if(!str.empty()){
        return stoi(str, index, base);
    }
    return 0;
}

void BaseTag::process(ProccessRect& rect){ //optimize and reorginize in the future 
    // Specific process

    // Block level element
    if(this->block_level){
        rect.accumulator.y += rect.max_height;
        rect.accumulator.x = rect.indent;
        rect.max_height = 0;
    }

    // Margin
    this->location_addition.x += safe_stoi(this->getAttribute(MARGIN_LEFT));
    this->location_addition.y += safe_stoi(this->getAttribute(MARGIN_TOP));
    this->location = rect.accumulator;

    // Padding
    rect.accumulator.x += safe_stoi(this->getAttribute(PADDING_LEFT));
    rect.indent += safe_stoi(this->getAttribute(PADDING_LEFT));
    rect.accumulator.y += safe_stoi(this->getAttribute(PADDING_TOP));

    // Recursive process
    Point children_bounding_box;
    Point starting_location = rect.accumulator;
    for(vector<BaseTag*>::const_iterator child_ptr = this->getFirstChild(); child_ptr != this->getLastChild(); child_ptr++){
        // Pass CSS style
        this->passStyle(*child_ptr);

        (*child_ptr)->process(rect);

        Point d = { (*child_ptr)->location.x + (*child_ptr)->bounding_box.x - starting_location.x,
                    (*child_ptr)->location.y + (*child_ptr)->bounding_box.y - starting_location.y };

                    children_bounding_box.x = (d.x > children_bounding_box.x) ? d.x : children_bounding_box.x;
                    children_bounding_box.y = (d.y > children_bounding_box.y) ? d.y : children_bounding_box.y;
    }

    // Aftermath process
    if(this->bounding_box.x == 0 && this->bounding_box.y == 0){
        this->bounding_box = children_bounding_box;
        if(this->block_level){

            // Apply HEIGHT, WIDTH
            string h = this->getAttribute(HEIGHT);
            string w = this->getAttribute(WIDTH);
            if(!w.empty()){
                if(w.at(0) != '%')  this->bounding_box.x = safe_stoi(w);
            }
            if(!h.empty()){
                if(h.at(0) != '%')  this->bounding_box.y = safe_stoi(h);
            }
        }
        bounding_box.x += safe_stoi(this->getAttribute(PADDING_RIGHT)) + safe_stoi(this->getAttribute(PADDING_LEFT));
        bounding_box.y += safe_stoi(this->getAttribute(PADDING_BOTTOM)) + safe_stoi(this->getAttribute(PADDING_TOP));
        rect.accumulator.x += safe_stoi(this->getAttribute(PADDING_RIGHT)) + safe_stoi(this->getAttribute(PADDING_LEFT));
    }
    else{
        bounding_box.x += safe_stoi(this->getAttribute(PADDING_RIGHT)) + safe_stoi(this->getAttribute(PADDING_LEFT));
        bounding_box.y += safe_stoi(this->getAttribute(PADDING_BOTTOM)) + safe_stoi(this->getAttribute(PADDING_TOP));
        rect.accumulator.x += this->bounding_box.x;
    }
    
    rect.max_height = (this->bounding_box.y > rect.max_height) ? this->bounding_box.y : rect.max_height;
    rect.accumulator.x += safe_stoi(this->getAttribute(MARGIN_LEFT)) + safe_stoi(this->getAttribute(MARGIN_RIGHT));
    rect.accumulator.y += safe_stoi(this->getAttribute(MARGIN_TOP)) + safe_stoi(this->getAttribute(MARGIN_BOTTOM));
    rect.indent -= safe_stoi(this->getAttribute(PADDING_LEFT));

    if(this->block_level){
        rect.accumulator.y += rect.max_height;
        rect.accumulator.x = 0;
        rect.max_height = 0;
    }
} 

void BaseTag::draw(DrawRect& rect){
    if(this->location_addition != Point()){
        this->addLocation(this->location_addition.x, this->location_addition.y);
        this->location_addition = Point();
    }

    // Basic draw
    if(this->block_level){
        string w = this->getAttribute(WIDTH);
        if(w.empty()) {
            this->bounding_box.x = rect.g_window.getWidth() - this->location.x - accumulateAttribute(PADDING_RIGHT) - accumulateAttribute(MARGIN_RIGHT);
        }
        else{
            if(w.at(0) == '%'){
                this->bounding_box.x = rect.g_window.getWidth() - this->location.x - accumulateAttribute(PADDING_RIGHT) - accumulateAttribute(MARGIN_RIGHT);
                this->bounding_box.x = this->bounding_box.x * safe_stoi(w.substr(1)) / 100;
            }
        }

        string h = this->getAttribute(HEIGHT);
        if(h.empty()){
            if(h.at(0) == '%'){
                this->bounding_box.y = this->bounding_box.y * safe_stoi(h.substr(1)) / 100;
            }
        }
    }

    SDL_Color raw_color = hexToColor(this->getAttribute(BACKGROUNDCOLOR));
    Uint32 color = SDL_MapRGB(rect.g_window.GetSurface()->format, raw_color.b, raw_color.g, raw_color.r);

    SDL_Rect draw_rect = {this->location.x, this->location.y, this->bounding_box.x, this->bounding_box.y};
    SDL_FillRect(rect.g_window.GetSurface(), &draw_rect, color);
    SDL_RenderDrawRect(rect.g_renderer.GetRaw(), &draw_rect);
    SDL_RenderClear(rect.g_renderer.GetRaw());

    // Specific draw

    // Recursive draw
    for(vector<BaseTag*>::const_iterator child_ptr = this->getFirstChild(); child_ptr != this->getLastChild(); child_ptr++){
        (*child_ptr)->draw(rect);
    }
}

void BaseTag::passStyle(BaseTag* child){
    child->addLocationAddition(this->location_addition.x, this->location_addition.y);
    for(int i = AttributeType::PASSABLE + 1; i < AttributeType::LAST; i++){
        this->passAttribute(child, (AttributeType)i);
    }
}

void BaseTag::passAttribute(BaseTag* child, AttributeType attribute){
    if(child->getAttribute(attribute) == ""){
        child->setAttribute(attribute, this->getAttribute(attribute));
    }
}

void BaseTag::addLocationAddition(int x, int y){
    this->location_addition.x += x;
    this->location_addition.y += y;
}

void BaseTag::setLocationAddition(int x, int y){
    this->location_addition.x = x;
    this->location_addition.y = y;
}

void BaseTag::addLocation(int x, int y){
    this->location.x += x;
    this->location.y += y;
}

void BaseTag::setLocation(int x, int y){
    this->location.x = x;
    this->location.y = y;
}

int BaseTag::accumulateAttribute(AttributeType attribute){
    int sum = 0;
    BaseTag* current = this;
    while(current != nullptr){
        sum += safe_stoi(current->getAttribute(attribute));
        current = current->getParent();
    }
    return sum;
}

SDL_Color BaseTag::hexToColor(string hex){
    if (hex[0] == '#')
        hex.erase(0, 1);

    unsigned long value = stoul(hex, nullptr, 16);

    SDL_Color color;

    color.a = (value >> 24) & 0xff;
    color.r = (value >> 16) & 0xff;
    color.g = (value >> 8) & 0xff;
    color.b = (value >> 0) & 0xff;
    return color;
}

void BaseTag::specificProccess(ProccessRect& rect) {}

void BaseTag::specificDraw(DrawRect& rect) {}