#include <iostream>
#include "data_loader.h"
#include "gray_image.h"
#include "image.h"
#include "rgb_image.h"

using namespace std;

int main(){
    Image *img1 = new RGBImage();
    img1->LoadImage("Image-Folder/myphoto.jpg");
    img1->DumpImage("img1.jpg");
    img1->Display_CMD(img1);
    img1->Display_ASCII(img1);

    delete img1;
    return 0;
}