#include <vector>
#include "data_loader.h"

int main() {
    Data_Loader data_loader;

    string file;

    // 1. Load gray image
    cout << "請輸入您的灰色圖片名稱：" ;
    cin >> file;

    string image_path = "Image-Folder/" + file;
    int w1;
    int h1;
    int **pixels1 = data_loader.Load_Gray(image_path, &w1, &h1);
    data_loader.Dump_Gray(w1, h1, pixels1, string("pixels1.jpg"));
    data_loader.Display_Gray_CMD("pixels1.jpg");

    for (int i = 0; i < h1; ++i) {
        delete[] pixels1[i];
    }
    delete[] pixels1;

    // 2. Load rgb image into gray scale image
    cout << "請輸入您的彩色圖片名稱：" ;
    cin >> file;

    image_path = "Image-Folder/" + file;
    int w2;
    int h2;
    int **pixels2 = data_loader.Load_Gray(image_path, &w2, &h2);
    data_loader.Dump_Gray(w2, h2, pixels2, string("pixels2.jpg"));
    data_loader.Display_Gray_CMD("pixels2.jpg");

    for (int i = 0; i < h2; ++i) {
        delete[] pixels2[i];
    }
    delete[] pixels2;

    // 3. Load rgb image
    cout << "請輸入您的彩色圖片名稱：" ;
    cin >> file;

    image_path = "Image-Folder/" + file;
    int w3;
    int h3;
    int ***pixels3 = data_loader.Load_RGB(image_path, &w3, &h3);
    data_loader.Dump_RGB(w3, h3, pixels3, string("pixels3.jpg"));
    data_loader.Display_RGB_ASCII(w3, h3, pixels3);
    data_loader.Display_RGB_CMD("pixels3.jpg");

    for (int i = 0; i < h3; ++i) {
        for (int j = 0; j < w3; ++j) {
            delete[] pixels3[i][j];
        }
        delete[] pixels3[i];
    }
    delete[] pixels3;

    // 4. List Directory
    vector<string> filenames;
    data_loader.List_Directory("Image-Folder/mnist", filenames);
    for(const auto &filename : filenames){
        cout << filename << endl;
    }

    return 0;
}
