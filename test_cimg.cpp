#include "third-party/CImg/CImg.h"
#include <iostream>

using namespace cimg_library;

int main() {
    try {
        const char* filename = "Image-Folder/1.jpeg";
        std::cout << "Loading file: " << filename << std::endl;
        CImg<uint8_t> image;
        image.load_jpeg(filename);
        std::cout << "Image loaded successfully." << std::endl;
    } catch (const cimg_library::CImgIOException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}