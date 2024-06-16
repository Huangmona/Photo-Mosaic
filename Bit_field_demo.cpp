#include "../inc/bit_field_filter.h"
#include "../Data-Loader/data_loader.h"
#include <iostream>
#include <bitset>

using namespace std;

void saveAndDisplay(Data_Loader& data_loader, int*** pixels, int width, int height, const string& filename) {
    data_loader.Dump_RGB(width, height, pixels, filename);
    data_loader.Display_RGB_CMD(filename);
}

int*** copyPixels(int*** pixels, int width, int height) {
    int*** copy = new int**[height];
    for (int i = 0; i < height; ++i) {
        copy[i] = new int*[width];
        for (int j = 0; j < width; ++j) {
            copy[i][j] = new int[3];
            for (int c = 0; c < 3; ++c) {
                copy[i][j][c] = pixels[i][j][c];
            }
        }
    }
    return copy;
}

void freePixels(int*** pixels, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            delete[] pixels[i][j];
        }
        delete[] pixels[i];
    }
    delete[] pixels;
}

void printFilterOptions() {
    cout << "您可選擇以下的濾鏡:" << endl;
    cout << "1. Fisheye" << endl;
    cout << "2. Contrast" << endl;
    cout << "3. Duotone" << endl;
    cout << "4. Blur" << endl;
    cout << "輸入數字1-4添加您想要的效果，可以選擇多個效果，輸入0為添加完畢" << endl;
}

int main() {
    Data_Loader data_loader;
    string file;
    cout << "請輸入您的圖片名稱：" ;
    cin >> file;

    string image_path = "Image-Folder/"+file;

    int width, height;

    int*** pixels = data_loader.Load_RGB(image_path, &width, &height);
    if (!pixels) {
        cerr << "Error: File " << image_path << " does not exist or could not be read." << endl;
        return 1;
    }

    uint8_t filter_code = 0; 
    int choice;
    printFilterOptions();
    while (true) {
        cin >> choice;
        if (choice == 0) break;
        switch (choice) {
            case 1: filter_code |= FISHEYE; break;
            case 2: filter_code |= CONTRAST; break;
            case 3: filter_code |= DUOTONE; break;
            case 4: filter_code |= BLUR; break;
            default: cout << "Invalid choice, try again." << endl;
        }
    }

    if (filter_code != 0) {
        int*** originalPixels = copyPixels(pixels, width, height);

        applyFilters(pixels, width, height, filter_code);

        string filename = "result_" + to_string(filter_code) + ".jpg";

        saveAndDisplay(data_loader, pixels, width, height, filename);

        freePixels(pixels, width, height);
        pixels = originalPixels;
    } else {
        cout << "No filters applied." << endl;
    }

    freePixels(pixels, width, height);

    return 0;
}
