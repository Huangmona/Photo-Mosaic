#include "../inc/bit_field_filter.h"

CImg<unsigned char> applyFilters(const CImg<unsigned char>& image, uint8_t filter_code) {
    CImg<unsigned char> result = image;

    if (filter_code & SHARPEN) {
        // Apply sharpen filter
        CImg<float> sharpen_kernel(3, 3, 1, 1, 0);
        sharpen_kernel(0, 1) = -1;
        sharpen_kernel(1, 0) = -1;
        sharpen_kernel(1, 1) = 5;
        sharpen_kernel(1, 2) = -1;
        sharpen_kernel(2, 1) = -1;
        result = result.get_convolve(sharpen_kernel);
    }

    if (filter_code & PIXELATE) {
        // Apply pixelate effect
        int pixel_size = 10;
        result.resize(result.width() / pixel_size, result.height() / pixel_size, -100, -100, 3);
        result.resize(result.width() * pixel_size, result.height() * pixel_size, -100, -100, 0);
    }

    if (filter_code & EXTRACT_LINES) {
        // Extract vertical and horizontal lines
        CImg<unsigned char> gray = result.get_RGBtoYCbCr().channel(0);
        CImgList<float> gradients = gray.get_gradient("xy");
        CImg<float> grad_x = gradients[0];
        CImg<float> grad_y = gradients[1];
        result = (grad_x + grad_y).normalize(0, 255).get_cut(0, 255);
    }

    if (filter_code & BLUR) {
        // Apply blur filter
        result.blur(2.5);
    }

    return result;
}