#pragma one

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include "globals.h"


enum class FontStyle {
    BLACK,
    BLACK_ITALIC,
    BOLD,
    BOLD_ITALIC,
    ITALIC,
    LIGHT,
    LIGHT_ITALIC,
    MEDIUM,
    MEDIUM_ITALIC,
    REGULAR,
    THIN,
    THIN_ITALIC
};

class FontManager {
    public:
        static FontManager& getInstance();

        FontManager(const FontManager&) = delete;

        TTF_Font* getFont(const std::string& fontName, int size, FontStyle style = FontStyle::REGULAR);

        ~FontManager(); 

    private:
        FontManager();

        std::vector<std::string> fontPaths;

        std::map<std::tuple<std::string, FontStyle, int>, TTF_Font*> fontCache;

        TTF_Font* loadFont(const std::string& fontPath, int size, int index = 0);

        std::string getStyleString(FontStyle style);
};