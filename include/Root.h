#pragma once
#include "BaseTag.h"
class Root :public BaseTag{
	public:
		Root();
		// Adapt to SDL
		// void rootProccess(TTF_Font* font);
		// void rootDraw(sf::RenderWindow& window);
		void specificProccess(ProccessRect& rect);
	    void specificDraw(DrawRect& rect);
};