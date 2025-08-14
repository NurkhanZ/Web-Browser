#include "Font_TTCParsing.h"

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
        if(error == FT_Err_Unknown_File_Format){
            std::cerr << "The file format is not supported" << std::endl;
            break;
        }
        else if(error){
            std::cerr <<"The font couldn't be opened or broken" << std::endl;
            break;
        }

        std::cout << "Face " << face_index << ": " << face->style_name << std::endl;

        std::string styleName = face->style_name;
        if(styleName.find("Bold") != std::string::npos){
            std::cout << "Found 'Bold' style at index " << face_index << std::endl;
        }

        face_index++;
        FT_Done_Face(face);
    }
    FT_Done_FreeType(library);
}

int findFontStyleIndex(const char* filePath, const char* styleName){
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if(error){
        std::cerr << "Could not initialize FreeType library." << std::endl;
        return -1;
    }

    FT_Face face;
    int face_index = 0;

    while(true){
        error = FT_New_Face(library, filePath, face_index, &face);
        if(error == FT_Err_Unknown_File_Format){
            std::cerr << "The file format is not supported" << std::endl;
            break;
        }
        else if(error){
            std::cerr <<"The font couldn't be opened or broken" << std::endl;
            break;
        }

        std::string styleName = face->style_name;
        if(styleName.find(styleName) != std::string::npos){
            FT_Done_Face(face);
            FT_Done_FreeType(library);
            return face_index;
        }

        face_index++;
        FT_Done_Face(face);
    }
    FT_Done_FreeType(library);
    return -1;
}