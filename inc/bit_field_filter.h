#ifndef BIT_FIELD_FILTER_H
#define BIT_FIELD_FILTER_H

#include <cstdint>

#define FISHEYE 0b00000001
#define CONTRAST 0b00000010
#define DUOTONE 0b00000100
#define BLUR 0b00001000

void applyFisheyeEffect(int*** pixels, int width, int height);
void applyContrastEffect(int*** pixels, int width, int height);
void applyDuotoneEffect(int*** pixels, int width, int height, unsigned char color1[3], unsigned char color2[3]);
void applyBlurEffect(int*** pixels, int width, int height);
void applyFilters(int*** pixels, int width, int height, uint8_t filter_code);

#endif // BIT_FIELD_FILTER_H
