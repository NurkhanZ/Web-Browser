#include "Root.h"

Root::Root(){
    this->type = TagType::ROOT;
    this->setAttribute(BOLD, "0");
    this->setAttribute(ITALIC, "0");
	this->setAttribute(UNDERLINED, "0");
	this->setAttribute(MARGIN_TOP, "8");
	this->setAttribute(MARGIN_LEFT, "8");
	this->setAttribute(MARGIN_BOTTOM, "8");
	this->setAttribute(MARGIN_RIGHT, "8");
	this->setAttribute(FONTSIZE, "16");
	this->setAttribute(COLOR, "000000");
	this->setAttribute(BACKGROUNDCOLOR, "ffffff");
}

void Root::specificProccess(ProccessRect& rect) {}

void Root::specificDraw(DrawRect& rect) {}