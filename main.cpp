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

#include "inc/bit_field_filter.h"
#include <iostream>
#include "Data-Loader/data_loader.h"
#include "inc/photo_mosaic.h"
#include <vector>
#include <string>

using namespace std;

int main() {
    Data_Loader data_loader; 
    
    // 使用 Data_Loader 加载小图像的路径
    vector<string> tile_image_paths;
    data_loader.List_Directory("Image-Folder/cifar10", tile_image_paths);

    // 创建 PhotoMosaic 实例
    PhotoMosaic mosaic("Image-Folder/1.jpeg", tile_image_paths, 32);

    // 生成照片马赛克
    mosaic.createMosaic("mosaic_output.png");

    return 0;
}
