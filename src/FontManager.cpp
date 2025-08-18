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

    for(const auto& baseDir: fontPaths){
        for(const auto& entry : fs::recursive_directory_iterator(baseDir)){
            // debug

            if(entry.path().filename().string().find(fontName) != std::string::npos && 
            (entry.path().extension() == ".ttf" || entry.path().extension() == ".ttc")) {
                // debug

                if(entry.path().extension() == ".ttc"){
                    try{
                        int index = findFontStyleIndex(entry.path().string().c_str(), getStyleString(style).c_str());
                        
                        TTF_Font* font = loadFont(entry.path().string(), size, index);

                        if(font){
                            fontCache[key] = font;
                            return font;
                        }
                    } catch(const std::exception& e){
                        std::cerr << "Error loading font: " << e.what() << std::endl; 
                    }
                }

                try{
                    TTF_Font* font = loadFont(entry.path().string(), size);
                    if(style == FontStyle::BOLD){
                        TTF_SetFontStyle(font, TTF_STYLE_BOLD);
                    } else if(style == FontStyle::ITALIC){
                        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
                    } else if(style == FontStyle::BOLD_ITALIC){
                        TTF_SetFontStyle(font, TTF_STYLE_BOLD || TTF_STYLE_ITALIC);
                    } else if(style == FontStyle::REGULAR){
                        TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
                    }
                    if(font){
                        fontCache[key] = font;
                        return font;
                    }
                } catch(const std::exception& e){
                    std::cerr << "Error loading font: " << e.what() << std::endl;
                }
            }
        }
    }

    static bool fontNotFound = false;
    if(!fontNotFound){
        std::cerr << "Font not found: " << fontName << std::endl;
        std::cerr << "Using default font" << std::endl;
        fontNotFound = true;
    }
    return getFont("arial", size, style);

    return nullptr;
}

TTF_Font* FontManager::loadFont(const std::string& fontPath, int size, int index){
    // debug

    if(!fs::exists(fontPath)){
        throw std::runtime_error("Font file doesn't exitst " + fontPath);
    }
    TTF_Font* font = TTF_OpenFontIndex(fontPath.c_str(), size, index);
    if(!font){
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return font;
}

std::string FontManager::getStyleString(FontStyle style){
    switch (style){
        case FontStyle::BLACK: return "Black";
        case FontStyle::BLACK_ITALIC: return "BlackItalic";
        case FontStyle::BOLD: return "Bold";
        case FontStyle::BOLD_ITALIC: return "BoldItalic";
        case FontStyle::ITALIC: return "Italic";
        case FontStyle::LIGHT: return "Light";
        case FontStyle::LIGHT_ITALIC: return "LightItalic";
        case FontStyle::MEDIUM: return "Medium";
        case FontStyle::MEDIUM_ITALIC: return "MediumItalic";
        case FontStyle::REGULAR: return "Regular";
        case FontStyle::THIN: return "Thin";
        case FontStyle::THIN_ITALIC: return "ThinItalic";
        default: return "Regular";
    }
}