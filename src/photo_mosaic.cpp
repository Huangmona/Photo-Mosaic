#include "photo_mosaic.h"
#include <iostream>
#include <cassert>

using namespace std;

void PhotoMosaic::createMosaic(const vector<string>& imagePaths, const string& outputFilename, int tileWidth, int tileHeight) {
    assert(!imagePaths.empty() && tileWidth > 0 && tileHeight > 0);

    // Load the first image to determine the overall mosaic dimensions
    RGBImage tile;
    if (!tile.LoadImage(imagePaths[0])) {
        cerr << "Failed to load image: " << imagePaths[0] << endl;
        return;
    }

    int imageWidth = tile.get_w();
    int imageHeight = tile.get_h();

    // Determine the number of tiles in the mosaic
    int tilesX = imageWidth / tileWidth;
    int tilesY = imageHeight / tileHeight;

    // Create the mosaic image
    CImg<unsigned char> mosaic(imageWidth, imageHeight, 1, 3);

    // Loop through each tile position
    for (int y = 0; y < tilesY; ++y) {
        for (int x = 0; x < tilesX; ++x) {
            int index = (y * tilesX + x) % imagePaths.size();
            const string& imagePath = imagePaths[index];

            if (!tile.LoadImage(imagePath)) {
                cerr << "Failed to load image: " << imagePath << endl;
                return;
            }

            CImg<unsigned char> resizedTile = tile.getCImg().resize(tileWidth, tileHeight);

            // Copy the resized tile into the mosaic
            mosaic.draw_image(x * tileWidth, y * tileHeight, resizedTile);
        }
    }

    // Save the mosaic image
    mosaic.save(outputFilename.c_str());
}
