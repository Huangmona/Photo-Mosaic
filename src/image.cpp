#include "image.h"

using namespace std;

Data_Loader Image::data_loader;

Image::Image() : w(0), h(0) {}

Image::Image(int width, int height) : w(width), h(height) {}

Image::~Image() {}

int Image::get_w() {
    return w;
}

int Image::get_h() {
    return h;
}
