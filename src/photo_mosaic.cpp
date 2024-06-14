#include "photo_mosaic.h"
#include <cmath>
#include <limits>

PhotoMosaic::PhotoMosaic(const string& target_image_path, const vector<string>& tile_image_paths, int tile_size)
    : target_image(target_image_path.c_str()), tile_size(tile_size) {
    for (const auto& path : tile_image_paths) {
        tile_images.emplace_back(path.c_str());
    }
}

void PhotoMosaic::createMosaic(const string& output_path) {
    CImg<unsigned char> mosaic(target_image.width(), target_image.height(), 1, 3, 0);
    for (int y = 0; y < target_image.height(); y += tile_size) {
        for (int x = 0; x < target_image.width(); x += tile_size) {
            CImg<unsigned char> best_tile = getBestMatchTile(x, y);
            mosaic.draw_image(x, y, best_tile);
        }
    }
    mosaic.save(output_path.c_str());
}

CImg<unsigned char> PhotoMosaic::getBestMatchTile(int x, int y) {
    CImg<unsigned char> target_tile = target_image.get_crop(x, y, x + tile_size - 1, y + tile_size - 1);
    double min_diff = numeric_limits<double>::max();
    CImg<unsigned char> best_tile;

    for (const auto& tile : tile_images) {
        CImg<unsigned char> resized_tile = tile;
        if (tile.width() != tile_size || tile.height() != tile_size) {
            resized_tile.resize(tile_size, tile_size);
        }
        double diff = calculateDiff(target_tile, resized_tile);
        if (diff < min_diff) {
            min_diff = diff;
            best_tile = resized_tile;
        }
    }
    return best_tile;
}

double PhotoMosaic::calculateDiff(const CImg<unsigned char>& img1, const CImg<unsigned char>& img2) {
    double diff = 0.0;
    for (int y = 0; y < img1.height(); ++y) {
        for (int x = 0; x < img1.width(); ++x) {
            for (int c = 0; c < 3; ++c) {
                int delta = img1(x, y, 0, c) - img2(x, y, 0, c);
                diff += delta * delta;
            }
        }
    }
    return sqrt(diff);
}
