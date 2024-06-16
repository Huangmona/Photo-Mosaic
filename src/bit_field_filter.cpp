#include "../inc/bit_field_filter.h"
#include <cmath>
#include <algorithm>

using namespace std;

void applyFisheyeEffect(int*** pixels, int width, int height) {
    int centerX = width / 2;
    int centerY = height / 2;
    int*** temp = new int**[height];

    for (int i = 0; i < height; ++i) {
        temp[i] = new int*[width];
        for (int j = 0; j < width; ++j) {
            temp[i][j] = new int[3]();
        }
    }

    float a = 1.008;
    float ncr = 0.01;
    float cr = height * 0.5;
    float rf = -1 * (log(1 * (1 - pow(a, -height)))) / (log(a));

    for (int y = 0; y < height; ++y) {
        int new_y = y - centerY;
        for (int x = 0; x < width; ++x) {
            int new_x = x - centerX;
            if (new_x == 0 && new_y == 0) continue;

            float fr = sqrt(pow(new_y, 2) + pow(new_x, 2));
            if (fr > cr) continue;

            float RaidX = acos(new_x / fr);
            float RaidY = asin(-new_y / fr);
            float r = 0;

            if (fr < cr * ncr) {
                r = fr * rf / (cr * ncr);
            } else {
                r = (log((cr * pow(a, cr)) / (cr * pow(a, cr) - fr * (pow(a, cr) - 1)))) / log(a);
            }

            int temp_x = static_cast<int>(r * cos(RaidX)) + centerX;
            int temp_y = centerY - static_cast<int>(r * sin(RaidY));

            if (temp_x >= 0 && temp_x < width && temp_y >= 0 && temp_y < height) {
                for (int c = 0; c < 3; ++c) {
                    temp[y][x][c] = pixels[temp_y][temp_x][c];
                }
            }
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 3; ++c) {
                pixels[y][x][c] = temp[y][x][c];
            }
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            delete[] temp[i][j];
        }
        delete[] temp[i];
    }
    delete[] temp;
}


void applyContrastEffect(int*** pixels, int width, int height) {
    double contrastFactor = 2.2; 

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 3; ++c) {
                int pixelValue = pixels[y][x][c];
                pixelValue = (int)((pixelValue - 128) * contrastFactor + 128);
                pixels[y][x][c] = pixelValue > 255 ? 255 : (pixelValue < 0 ? 0 : pixelValue);
            }
        }
    }
}


void applyDuotoneEffect(int*** pixels, int width, int height, unsigned char color1[3], unsigned char color2[3]) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char gray = (pixels[y][x][0] + pixels[y][x][1] + pixels[y][x][2]) / 3;
            int red = (gray * color2[0] + (255 - gray) * color1[0]) / 255 + 10;
            int green = (gray * color2[1] + (255 - gray) * color1[1]) / 255 + 10;
            int blue = (gray * color2[2] + (255 - gray) * color1[2]) / 255 + 10;
            
            pixels[y][x][0] = (red > 255) ? 255 : red;
            pixels[y][x][1] = (green > 255) ? 255 : green;
            pixels[y][x][2] = (blue > 255) ? 255 : blue;
        }
    }
}


void applyBlurEffect(int*** pixels, int width, int height) {
    int kernelSize = 5; // 增加模糊效果的核大小
    int*** temp = new int**[height];

    for (int i = 0; i < height; ++i) {
        temp[i] = new int*[width];
        for (int j = 0; j < width; ++j) {
            temp[i][j] = new int[3]();
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum[3] = {0, 0, 0};
            int count = 0;
            for (int ky = -kernelSize; ky <= kernelSize; ++ky) {
                for (int kx = -kernelSize; kx <= kernelSize; ++kx) {
                    int iy = y + ky;
                    int ix = x + kx;
                    if (iy >= 0 && iy < height && ix >= 0 && ix < width) {
                        for (int c = 0; c < 3; ++c) {
                            sum[c] += pixels[iy][ix][c];
                        }
                        ++count;
                    }
                }
            }
            for (int c = 0; c < 3; ++c) {
                temp[y][x][c] = sum[c] / count;
            }
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 3; ++c) {
                pixels[y][x][c] = temp[y][x][c];
            }
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            delete[] temp[i][j];
        }
        delete[] temp[i];
    }
    delete[] temp;
}

void applyFilters(int*** pixels, int width, int height, uint8_t filter_code) {
    if (filter_code & FISHEYE) {
        applyFisheyeEffect(pixels, width, height);
    }
    if (filter_code & CONTRAST) {
        applyContrastEffect(pixels, width, height);
    }
    if (filter_code & DUOTONE) {
        unsigned char color1[3] = {255, 0, 0}; 
        unsigned char color2[3] = {0, 0, 0};   
        applyDuotoneEffect(pixels, width, height, color1, color2);
    }
    if (filter_code & BLUR) {
        applyBlurEffect(pixels, width, height);
    }
}