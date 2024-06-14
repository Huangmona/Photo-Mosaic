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
#include <fstream>

using namespace std;

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

int main() {
    Data_Loader data_loader; 
    
    string target_image_path = "Image-Folder/myphoto.jpg";
    if (!fileExists(target_image_path)) {
        cerr << "Error: File " << target_image_path << " does not exist." << endl;
        return 1;
    }
    cout << "Target image file exists: " << target_image_path << endl;

    vector<string> tile_image_paths;
    data_loader.List_Directory("Image-Folder/cifar10", tile_image_paths);


    for (const auto& path : tile_image_paths) {
        if (!fileExists(path)) {
            cerr << "Error: Tile image file " << path << " does not exist." << endl;
            return 1;
        }
        cout << "v" ;
    }
    cout << endl;


    try {
        PhotoMosaic mosaic(target_image_path, tile_image_paths, 32);
        mosaic.createMosaic("mosaic_output.jpg");
    } catch (const cimg_library::CImgIOException& e) {
        cerr << "CImgIOException: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
