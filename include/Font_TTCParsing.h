#pragma once

#include <iostream>
#include <freetype2/ft2build.h>
#include <freetype2/freetype/freetype.h>

void printFontStyles(const char* filePath);

int findFontStyleIndex(const char* filePath, const char* styleName);