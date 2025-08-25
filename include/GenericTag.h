#pragma once
#include "BaseTag.h"

class GenericTag : public BaseTag{
    public:
        GenericTag();
        GenericTag(string type);
        BaseTag* specificCopy();
        void specificProccess(ProccessRect& rect);
        void specificDraw(DrawRect& rect);
        inline bool isType(string type) const {return type == this->type_string; };
    private:
        string type_string;
};