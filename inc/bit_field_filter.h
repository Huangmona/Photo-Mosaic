#ifndef BIT_FIELD_FILTER_H
#define BIT_FIELD_FILTER_H

#include "../third-party/CImg/CImg.h"
#include <cstdint>

using namespace cimg_library;

#define SHARPEN 0b00000001
#define PIXELATE 0b00000010
#define EXTRACT_LINES 0b00000100
#define BLUR 0b00001000

CImg<unsigned char> applyFilters(const CImg<unsigned char>& image, uint8_t filter_code);

#endif // BIT_FIELD_FILTER_H
