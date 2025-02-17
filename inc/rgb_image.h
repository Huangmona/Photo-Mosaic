#ifndef RGB_IMAGE_H
#define RGB_IMAGE_H

#include "image.h"
#include <string>

using namespace std;

class RGBImage : public Image {
private:
    int ***pixels; // 3D array for RGB pixels

public:
    RGBImage();
    RGBImage(int width, int height, int*** pix);
    ~RGBImage();

    bool LoadImage(string& filename) override;
    void DumpImage(string& filename) override;
    void Display_ASCII() override;
    void Display_CMD() override;
};

#endif // RGB_IMAGE_H
