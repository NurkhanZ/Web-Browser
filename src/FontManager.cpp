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