#ifndef BASETAG
#define BASETAG

#pragma once
#include <vector>
#include <map>
// #include <SFML/Graphics.hpp> instead i use sdl
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream> // Debugging
#include <string>
#include "SmartWindow.h"
#include "SmartRenderer.h"
using namespace std;

enum TagType{
    ROOT,
    GENERIC,
    TEXT,
    BR,
    B,
    I,
    BODY,
    P,
    H1, H2, H3, H4, H5, H6,
    UNDEFINED
};

enum AttributeType{
    NONE,
	CLASS,
	ID,
	STYLE,
	WIDTH,
	HEIGHT,
	MARGIN_LEFT,
	MARGIN_TOP,
	MARGIN_RIGHT,
	MARGIN_BOTTOM,
	PADDING_LEFT,
	PADDING_TOP,
	PADDING_RIGHT,
	PADDING_BOTTOM,
	TEXT_CONTENT,
	BACKGROUNDCOLOR,
	PASSABLE,
	FONTSIZE,
	COLOR,
	BOLD,
	ITALIC,
	UNDERLINED,
	LAST
};

// color more convenient way of applying through switch
enum ColorType{
    WHITE,
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW
};

// struct Point and some operator functions
struct Point{
    int x = 0;
    int y = 0;
    
    Point(){}
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }

    inline void operator+=(const Point& additive){
        this->x += additive.x;
        this->y += additive.y;
    }

    inline void operator-=(const Point& substractive){
        this->x -= substractive.x;
        this->y -= substractive.y;
    }
};


inline Point operator+(Point me, const Point& additive){
    me.x += additive.x;
    me.y += additive.y;
    return me;
}


inline Point operator-(Point me, const Point& substractive){
    me.x -= substractive.x;
    me.y -= substractive.y;
    return me;
}

inline bool operator==(const Point& left, const Point& right){
    return left == right;
}

inline bool operator!=(const Point& left, const Point& right){
    return !(left == right);
}

// struct ProcessContext? and struct DrawContext
struct ProccessRect{
    Point accumulator;
    int max_height = 0;
    int indent = 0;
    TTF_Font* font = nullptr; // figure how to adapt to sdl_ttf
    ProccessRect(TTF_Font* new_font) : font(new_font) {}
};

struct DrawRect{
    SmartWindow g_window;
    SmartRenderer g_renderer;
};

class BaseTag{
    public:
        BaseTag();
        TagType type;
        bool standalone;

        // Parser
        BaseTag* addChild(BaseTag* child);
        static TagType stringToType(string type);
        BaseTag* copy();

        inline BaseTag* getParent() const { return this->parent; };
        inline vector<BaseTag*>::const_iterator getFirstChild() const {return this->children.begin(); };
        inline vector<BaseTag*>::const_iterator getLastChild() const {return this->children.end(); };
        inline virtual bool isType(string type) const { return this->type == stringToType(type); };
        inline void setAttribute(AttributeType type, string value){ this->attributes[type] = value; };

        // Processor
        string getAttribute(AttributeType type);
        static int safe_stoi(string str, size_t* index = 0, int base = 10);
        inline void setLineBreak(bool linebreak){ this->block_level = linebreak; };
        void process(ProccessRect& rect); // adapt to sdl, it has inner workings similar to SDL_Rect

        // Renderer
        static SDL_Color hexToColor(string hex);
        void draw(DrawRect& rect); // adapt to sdl, similar to SDL_RenderCopy

        virtual void specificProccess(ProccessRect& rect);
	    virtual void specificDraw(DrawRect& rect);

    protected:
        // what if all params except TagType is private?
        int index;
	    bool block_level; // not inline element
        bool wrapping;
        BaseTag* parent;
        vector<BaseTag*> children;
        map<AttributeType, string> attributes;
        Point location, location_addition, bounding_box;

        // Processor
        void passStyle(BaseTag* child);
        void passAttribute(BaseTag* child, AttributeType attribute);
        void addLocationAddition(int x, int y);
        void setLocationAddition(int x, int y);
        void addLocation(int x, int y);
        void setLocation(int x, int y);

        virtual BaseTag* specificCopy();
        

        // Renderer
        int accumulateAttribute(AttributeType attribute);
};

#endif