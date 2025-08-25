#pragma once
#include "BaseTag.h"
class BreakTag : public BaseTag{
public:
	BreakTag();
	void specificProccess(ProccessRect& rect);
	void specificDraw(DrawRect& rect);
};