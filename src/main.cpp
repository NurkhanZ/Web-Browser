#include "Parser.hpp"
#include "Renderer.h"
#include <iostream>

using namespace std;

// Instructions for todays practice:

// 1. Files to modify: SmartFont.h, SmartFont.cpp, Parser.cpp

// 2. Files to create: BoldTag.h, ItalicTag.h, BodyTag.h, H1Tag.h, H2Tag.h, H3Tag.h, H4Tag.h, H5Tag.h, H6Tag.h,
//                     BoldTag.cpp, ItalicTag.cpp, BodyTag.cpp, H1Tag.cpp, H2Tag.cpp, H3Tag.cpp, H4Tag.cpp, H5Tag.cpp, H6Tag.cpp,

// 3. Try to prototype and pseudocode:

//               Constructor of these files, 

//               specificDraw() and specificProcess() functions in .cpp files. 

//               (Look up BaseTag.h and BaseTag.cpp, these have all base parameters that html tag should've)

// 4. If you see commented code, most likely it needs implementation, try to pseudocode it.

// P.S. To compile project, type in console: cd build
//                                           cmake ../
//                                           cmake --build .

//  Good luck!


int main(int argc, char* argv[]){
    string filePath = "index.html";
    const char* filePathChar = filePath.c_str(); 
    if (argc >= 2) {
        filePath = argv[1];
    }

    Parser parser(filePath);
    // parser.parse();
    
    // Renderer renderer(parser.getRoot(), parser.getTitle());
    // renderer.processTree();
    // renderer.drawTree();
    // renderer.maintainWindow();


    return 0;
}