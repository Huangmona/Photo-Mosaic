#ifndef PHOTO_MOSAIC_H
#define PHOTO_MOSAIC_H

#include "../third-party/CImg/CImg.h"
#include "data_loader.h"
#include <vector>
#include <string>

using namespace std;
using namespace cimg_library;

class PhotoMosaic {
public:
    PhotoMosaic(const string& target_image_path, const vector<string>& tile_image_paths, int tile_size);
    void createMosaic(const string& output_path);

private:
    CImg<unsigned char> target_image;
    vector<CImg<unsigned char>> tile_images;
    int tile_size;
    Data_Loader data_loader;

    CImg<unsigned char> getBestMatchTile(int x, int y);
    double calculateDiff(const CImg<unsigned char>& img1, const CImg<unsigned char>& img2);
};

#endif // PHOTO_MOSAIC_H
