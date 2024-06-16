#include "rgb_image.h"
#include "data_loader.h"
#include <cassert>

using namespace std;

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
        pixels = nullptr;
    }
}


bool RGBImage::LoadImage(string& filename) {
    Data_Loader data_loader;
    pixels = data_loader.Load_RGB(filename, &w, &h);
    return pixels != nullptr;
}

void RGBImage::DumpImage(string& filename) {
    Data_Loader data_loader;
    data_loader.Dump_RGB(w, h, pixels, filename);
}

void RGBImage::Display_ASCII() {
    Data_Loader data_loader;
    data_loader.Display_RGB_ASCII(w, h, pixels);
}

void RGBImage::Display_CMD() {
    string temp_filename = "temp_image.png";
    DumpImage(temp_filename);
    Data_Loader data_loader;
    data_loader.Display_RGB_CMD(temp_filename);
    remove(temp_filename.c_str());
}
