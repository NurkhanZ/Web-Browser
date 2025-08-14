#include "Font_TTCParsing.h"
#include <freetype2/freetype/freetype.h>

void printFontStyles(const char* filePath){
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if(error){
        std::cerr << "Could not initialize FreeType library." << std::endl;
    }

    FT_Face face;
    int face_index = 0;

    while(true){
        error = FT_New_Face(library, filePath, face_index, &face);
    }
}