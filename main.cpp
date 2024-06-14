/*#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "photo_mosaic.h"
#include "bit_field_filter.h"

int main(int argc, char *argv[]){
    Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/mnist/img_100.jpg");
    img1->DumpImage("img1.jpg");
    img1->Display_X_Server();
    img1->Display_CMD();
    

    Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/lena.jpg");
    img2->DumpImage("img2.jpg");
    img2->Display_X_Server();
    img2->Display_CMD();


    // some bit field filter design driven code here

    // some photo mosaic driven code here

    return 0;
}
*/

#include "Data-Loader/data_loader.h"
#include "inc/bit_field_filter.h"
#include <iostream>

int main() {
    Data_Loader data_loader;
    
    // 1. Load gray image
    int w1;
    int h1;
    int **pixels1 = data_loader.Load_Gray("Image-Folder/mnist/img_0.jpg", &w1, &h1);
    data_loader.Dump_Gray(w1, h1, pixels1, std::string("pixels1.jpg"));
    //data_loader.Display_Gray_X_Server(w1, h1, pixels1);
    data_loader.Display_Gray_ASCII(w1, h1, pixels1);
    data_loader.Display_Gray_CMD("pixels1.jpg");

    // 2. Load rgb image into gray scale image
    int w2;
    int h2;
    int **pixels2 = data_loader.Load_Gray("Image-Folder/lena.jpg", &w2, &h2);
    data_loader.Dump_Gray(w2, h2, pixels2, std::string("pixels2.jpg"));
    //data_loader.Display_Gray_X_Server(w2, h2, pixels2);
    data_loader.Display_Gray_CMD("pixels2.jpg");

    // 3. Load rgb image
    int w3;
    int h3;
    int ***pixels3 = data_loader.Load_RGB("Image-Folder/cifar10/airplane_0010.png", &w3, &h3);
    data_loader.Dump_RGB(w3, h3, pixels3, std::string("pixels3.jpg"));
    //data_loader.Display_RGB_X_Server(w3, h3, pixels3);
    data_loader.Display_RGB_ASCII(w3, h3, pixels3);
    data_loader.Display_RGB_CMD("pixels3.jpg");

    // Load an image using CImg
    CImg<unsigned char> image("Image-Folder/lena.jpg");

    // Define the filter code
    uint8_t filter_code = SHARPEN | PIXELATE | EXTRACT_LINES | BLUR;

    // Apply filters
    CImg<unsigned char> filtered_image = applyFilters(image, filter_code);

    // Save the result
    filtered_image.save("filtered_image.jpg");

    return 0;
}
