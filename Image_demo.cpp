#include "Data-Loader/data_loader.h"
#include "inc/rgb_image.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    Data_Loader dataLoader;
    int width, height;

    // Load the RGB image using Data_Loader
    string filename;
    cout << "請輸入您的彩色圖片名稱：";
    cin >> filename;

    int ***pixels = dataLoader.Load_RGB("Image-Folder/" + filename, &width, &height);

    if (!pixels) {
        cerr << "Failed to load RGB image." << endl;
        return 1;
    }

    // Create an instance of RGBImage
    RGBImage rgbImg(width, height, pixels);

    // Now you can use rgbImg as needed, for example, dumping the image
    string dumpFilename = "dump_rgb_image.jpg";
    rgbImg.DumpImage(dumpFilename);

    cout << "RGB image dumped to " << dumpFilename << endl;

    // rgbImg will automatically clean up the allocated memory when it goes out of scope
    return 0;
}
