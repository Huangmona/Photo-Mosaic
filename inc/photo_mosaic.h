#ifndef PHOTO_MOSAIC_H
#define PHOTO_MOSAIC_H

#include "../Data-Loader/data_loader.h"
#include <vector>
#include <string>

using namespace std;

class PhotoMosaic {
public:
    PhotoMosaic(const string& target_image_path, const vector<string>& tile_image_paths, int tile_size, Data_Loader& data_loader);
    ~PhotoMosaic();
    void createMosaic(const string& output_path);

private:
    int*** target_image_rgb;    // RGB图像
    int width, height;
    vector<int***> tile_images; // RGB图像
    vector<int> tile_widths;
    vector<int> tile_heights;
    int tile_size;
    Data_Loader& data_loader;  // 引用 Data_Loader 对象

    int*** getBestMatchTile(int x, int y);
    double calculateDiff(int*** img1, int*** img2, int w1, int h1, int x, int y, int w2, int h2);
    void applyColorCorrection(int*** tile_img, int*** target_rgb, int x, int y, int tile_w, int tile_h);
    int*** resizeTile(int*** pixels, int old_width, int old_height, int new_width, int new_height);
    void freePixels(int*** pixels, int width, int height); // 释放RGB图像内存
};

#endif // PHOTO_MOSAIC_H
