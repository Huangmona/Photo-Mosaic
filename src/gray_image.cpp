#include "gray_image.h"
#include <iostream>
#include <cassert>

using namespace std;

GrayImage::GrayImage() : pixels(nullptr) {}

GrayImage::GrayImage(int width, int height, int** pix) : Image(width, height), pixels(pix) {}

GrayImage::~GrayImage() {
    if (pixels) {
        for (int i = 0; i < h; ++i) {
            delete[] pixels[i];
        }
        delete[] pixels;
    }
}

bool GrayImage::LoadImage(string& filename) { 
    pixels = data_loader.Load_Gray(filename, &w, &h);
    return pixels != nullptr;
}

void GrayImage::DumpImage(string& filename) {
    data_loader.Dump_Gray(w, h, pixels, filename);
}

void GrayImage::Display_ASCII() {
    data_loader.Display_Gray_ASCII(w, h, pixels);
}

void GrayImage::Display_CMD() {
    // 暫存圖像
    string temp_filename = "temp_image.png";
    DumpImage(temp_filename);

    // 使用system調用catimg
    data_loader.Display_Gray_CMD(temp_filename);

    // 刪除暫存圖像
    remove(temp_filename.c_str());
}