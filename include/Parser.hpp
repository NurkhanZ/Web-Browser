#ifndef PARSER
#define PARSER

#pragma once
#include <iostream>
#include <fstream>
#include "TagSystem.h"
#include "Debug.h"

class Parser{
    public:
        Parser(std::string path);
        BaseTag* parse();
        inline Root* getRoot() const {return this->root;};
        inline std::string getTitle() const {return this->title;};
    private:
        Root* root;
        std::string title;

        std::ifstream file;
        BaseTag* current_tag;
        std::map<std::string, std::string> css;

        // Parser
        char current_char;
        char nextChar();
        void newMarkup();
        void newText();
        void closeTag(string type);
        bool createTag(string type);

        // Css
        string getCSS(string type);
        string getCSSByClass(string _class);
        string getCSSById(string id);
        void setAttribute(string key, string value);
        void setStyle(string raw_style);
        void addCSS(string raw_css);
        static string parseColor(string value);
        static string parseSize(string value);
        static string toLower(string str);
        static string cleanWhitespace(string raw);

};
#endif