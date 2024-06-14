#include <iostream>
#include "data_loader.h"
#include "gray_image.h"
#include "image.h"
#include "rgb_image.h"

using namespace std;

int main() {
    Image *img1 = new RGBImage();
    string filename = "Image-Folder/myphoto.jpg";
    string outputFilename = "img1.jpg";
    
    img1->LoadImage(filename);
    img1->DumpImage(outputFilename);
    img1->Display_CMD();
    img1->Display_ASCII();

    delete img1;
    return 0;
}
