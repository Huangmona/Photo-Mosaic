#include "rgb_image.h"
#include <cassert>

RGBImage::RGBImage() : pixels(nullptr) {}

RGBImage::RGBImage(int width, int height, int*** pix) : Image(width, height), pixels(pix) {}

RGBImage::~RGBImage() {
    if (pixels) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                delete[] pixels[i][j];
            }
            delete[] pixels[i];
        }
        delete[] pixels;
    }
}

bool RGBImage::LoadImage(string& filename) {
    pixels = data_loader.Load_RGB(filename, &w, &h);
    return pixels != nullptr;
}

void RGBImage::DumpImage(string& filename) {
    data_loader.Dump_RGB(w, h, pixels, filename);
}

void RGBImage::Display_ASCII() {
    data_loader.Display_RGB_ASCII(w, h, pixels);
}

void RGBImage::Display_CMD() {
    string temp_filename = "temp_image.png";
    DumpImage(temp_filename);
    data_loader.Display_RGB_CMD(temp_filename);
    remove(temp_filename.c_str());
}

CImg<unsigned char> RGBImage::getCImg() const {
    CImg<unsigned char> img(w, h, 1, 3);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            img(x, y, 0, 0) = pixels[y][x][0];
            img(x, y, 0, 1) = pixels[y][x][1];
            img(x, y, 0, 2) = pixels[y][x][2];
        }
    }
    return img;
}
