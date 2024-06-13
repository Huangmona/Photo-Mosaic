#ifndef GRAY_IMAGE_H
#define GRAY_IMAGE_H

#include "image.h"
#include <string>

using namespace std;

class GrayImage : public Image {
private:
    int** pixels;

public:
    GrayImage();
    GrayImage(int width, int height, int** pix);
    ~GrayImage();

    bool LoadImage(string& filename) override;
    void DumpImage(string& filename) override;
    void Display_ASCII() override;
    void Display_CMD() override;
};

#endif // GRAY_IMAGE_H
