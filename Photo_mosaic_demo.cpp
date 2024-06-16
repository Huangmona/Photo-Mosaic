#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "inc/bit_field_filter.h"
#include "Data-Loader/data_loader.h"
#include "inc/photo_mosaic.h"

using namespace std;

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

int main() {
    Data_Loader data_loader; 
    string file;
    int tile_size;

    // 用户输入图片名称
    cout << "請輸入您的圖片名稱：";
    cin >> file;

    // 用户输入瓦片大小
    cout << "請問您想要 1. 高清晰度 2. 中清晰度 3. 低清晰度？ 請輸入代碼：" ;
    int temp;
    cin >> temp;
    if(temp == 1)
        tile_size = 8;
    else if(temp == 2)
        tile_size = 16;
    else 
        tile_size = 32;

    string target_image_path = "Image-Folder/" + file;
    if (!fileExists(target_image_path)) {
        cerr << "Error: File " << target_image_path << " does not exist." << endl;
        return 1;
    }
    cout << "Target image file exists: " << target_image_path << endl;

    vector<string> tile_image_paths;
    if (data_loader.List_Directory("Image-Folder/cifar10", tile_image_paths) != 0) {
        cerr << "Error: Failed to list directory Image-Folder/cifar10." << endl;
        return 1;
    }

    for (const auto& path : tile_image_paths) {
        if (!fileExists(path)) {
            cerr << "Error: Tile image file " << path << " does not exist." << endl;
            return 1;
        }
    }
    cout << "All tile image files exist." << endl;

    try {
        PhotoMosaic mosaic(target_image_path, tile_image_paths, tile_size, data_loader);
        mosaic.createMosaic("mosaic_output.jpg");
        cout << "Mosaic created successfully: mosaic_output.jpg" << endl;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
