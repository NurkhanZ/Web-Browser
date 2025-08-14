#include "FontManager.h"

// #define DEBUG

namespace fs = std::filesystem;

std::string getHomeDirectory(){
    char* homeDir = getenv("HOME");
    if(homeDir == nullptr){
        homeDir = getenv("USERPROFILE");
    }
    return std::string(homeDir ? homeDir : "");
}

std::vector<std::string> getFontPaths(){
    std::vector<std::string> paths;
    #ifdef _WIN32
        paths.push_back("C:\\Windows\\Fonts");
    #elif defined(__APPLE__)
        paths.push_back("/Library/Fonts");
        paths.push_back("/System/Library/Fonts");
        paths.push_back(getHomeDirectory() + "/Library/Fonts");
    #else
        paths.push_back("/usr/share/fonts");
        paths.push_back("/usr/local/share/fonts");
        paths.push_back(getHomeDirectory() + "/.fonts");
    #endif
    return paths;
}

FontManager::FontManager(){
    // debug message

    fontPaths = getFontPaths();
}

FontManager::~FontManager(){
    // debug message

    for(auto& font: fontCache){
        TTF_CloseFont(font.second);
    }
    fontCache.clear();
}

FontManager& FontManager::getInstance(){
    static FontManager instance;
    return instance;
}

TTF_Font* FontManager::getFont(const std::string& fontName, int size, FontStyle style){
    #ifdef DEBUG
        std::cout << "FontManager getFont called, looking for " << fontName << std::endl;
    #endif

    auto key = std::make_tuple(fontName + getStyleString(style), style, size);
    if(fontCache.find(key) != fontCache.end()){
        return fontCache[key];
    }

    #ifdef DEBUG
        std::cout << "Font not found in cache" << std::endl;
    #endif
    
}