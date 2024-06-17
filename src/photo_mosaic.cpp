#include "../inc/photo_mosaic.h"
#include <cmath>
#include <cfloat> // For DBL_MAX
#include <iostream>
#include <vector>
#include <algorithm> // For std::min
#include <sstream>
#include <iomanip>

using namespace std;

PhotoMosaic::PhotoMosaic(const string& target_image_path, const vector<string>& tile_image_paths, int tile_size, Data_Loader& data_loader)
    : tile_size(tile_size), data_loader(data_loader) {

    target_image_rgb = data_loader.Load_RGB(target_image_path, &width, &height);
    cout << "Loaded target image: " << target_image_path << ", width: " << width << ", height: " << height << endl;

    for (const string& path : tile_image_paths) {
        int tile_w, tile_h;
        int*** tile_img = data_loader.Load_RGB(path, &tile_w, &tile_h);

        if (tile_w != tile_size || tile_h != tile_size) {
            int*** resized_tile_img = resizeTile(tile_img, tile_w, tile_h, tile_size, tile_size);
            freePixels(tile_img, tile_w, tile_h);
            tile_img = resized_tile_img;
            tile_w = tile_size;
            tile_h = tile_size;
        }
        tile_images.push_back(tile_img);
    }
}

PhotoMosaic::PhotoMosaic(const string& target_image_path, const string& tile_image_path, int tile_size, Data_Loader& data_loader)
    : tile_size(tile_size), data_loader(data_loader) {

    target_image_rgb = data_loader.Load_RGB(target_image_path, &width, &height);
    cout << "Loaded target image: " << target_image_path << ", width: " << width << ", height: " << height << endl;

    int tile_width, tile_height;
    int*** tile_image = data_loader.Load_RGB(tile_image_path, &tile_width, &tile_height);
    if (tile_width != tile_size || tile_height != tile_size) {
        int*** resized_tile_img = resizeTile(tile_image, tile_width, tile_height, tile_size, tile_size);
        freePixels(tile_image, tile_width, tile_height);
        tile_image = resized_tile_img;
        tile_width = tile_size;
        tile_height = tile_size;
    }
    vector<int***> tiles;
    splitImageToTiles(tile_image, tile_width, tile_height, tile_size, tiles);
    tile_images.insert(tile_images.end(), tiles.begin(), tiles.end());
    freePixels(tile_image, tile_width, tile_height);
}

PhotoMosaic::~PhotoMosaic() {
    freePixels(target_image_rgb, width, height);

    for (size_t i = 0; i < tile_images.size(); ++i) {
        freePixels(tile_images[i], tile_size, tile_size);
    }
}

void PhotoMosaic::splitImageToTiles(int*** image, int image_width, int image_height, int tile_size, vector<int***>& tiles) {
    for (int y = 0; y < image_height; y += tile_size) {
        for (int x = 0; x < image_width; x += tile_size) {
            int*** tile = new int**[tile_size];
            for (int dy = 0; dy < tile_size; ++dy) {
                tile[dy] = new int*[tile_size];
                for (int dx = 0; dx < tile_size; ++dx) {
                    tile[dy][dx] = new int[3];
                    for (int c = 0; c < 3; ++c) {
                        tile[dy][dx][c] = image[y + dy][x + dx][c];
                    }
                }
            }
            tiles.push_back(tile);
        }
    }
}

void PhotoMosaic::createMosaic(const string& output_path) {
    int mosaic_width = (width / tile_size) * tile_size;
    int mosaic_height = (height / tile_size) * tile_size;

    int*** mosaic_image = new int**[mosaic_height];
    for (int i = 0; i < mosaic_height; ++i) {
        mosaic_image[i] = new int*[mosaic_width];
        for (int j = 0; j < mosaic_width; ++j) {
            mosaic_image[i][j] = new int[3]();
        }
    }

    for (int y = 0; y < mosaic_height; y += tile_size) {
        for (int x = 0; x < mosaic_width; x += tile_size) {
            int*** best_match_tile = getBestMatchTile(x, y);
            applyColorCorrection(best_match_tile, target_image_rgb, x, y, tile_size, tile_size);
            for (int dy = 0; dy < tile_size; ++dy) {
                for (int dx = 0; dx < tile_size; ++dx) {
                    for (int c = 0; c < 3; ++c) {
                        mosaic_image[y + dy][x + dx][c] = best_match_tile[dy][dx][c];
                    }
                }
            }
        }
    }

    data_loader.Dump_RGB(mosaic_width, mosaic_height, mosaic_image, output_path);

    for (int i = 0; i < mosaic_height; ++i) {
        for (int j = 0; j < mosaic_width; ++j) {
            delete[] mosaic_image[i][j];
        }
        delete[] mosaic_image[i];
    }
    delete[] mosaic_image;
}

int*** PhotoMosaic::getBestMatchTile(int x, int y) {
    double min_diff = DBL_MAX;
    int*** best_match_tile = nullptr;

    for (size_t i = 0; i < tile_images.size(); ++i) {
        int*** tile_img = tile_images[i];
        double diff = calculateDiff(tile_img, target_image_rgb, tile_size, tile_size, x, y, tile_size, tile_size);
        if (diff < min_diff) {
            min_diff = diff;
            best_match_tile = tile_img;
        }
    }
    return best_match_tile;
}

double PhotoMosaic::calculateDiff(int*** img1, int*** img2, int w1, int h1, int x, int y, int w2, int h2) {
    double diff = 0.0;
    for (int dy = 0; dy < h2; ++dy) {
        for (int dx = 0; dx < w2; ++dx) {
            for (int c = 0; c < 3; ++c) {
                diff += pow(img1[dy][dx][c] - img2[y + dy][x + dx][c], 2);
            }
        }
    }
    return sqrt(diff);
}

void PhotoMosaic::applyColorCorrection(int*** tile_img, int*** target_rgb, int x, int y, int tile_w, int tile_h) {
    int avg_tile[3] = {0, 0, 0};
    for (int dy = 0; dy < tile_h; ++dy) {
        for (int dx = 0; dx < tile_w; ++dx) {
            for (int c = 0; c < 3; ++c) {
                avg_tile[c] += tile_img[dy][dx][c];
            }
        }
    }
        for (int c = 0; c < 3; ++c) {
        avg_tile[c] /= (tile_w * tile_h);
        if (avg_tile[c] == 0) avg_tile[c] = 1;
    }

    int avg_target[3] = {0, 0, 0};
    for (int dy = 0; dy < tile_h; ++dy) {
        for (int dx = 0; dx < tile_w; ++dx) {
            for (int c = 0; c < 3; ++c) {
                avg_target[c] += target_rgb[y + dy][x + dx][c];
            }
        }
    }
    for (int c = 0; c < 3; ++c) {
        avg_target[c] /= (tile_w * tile_h);
        if (avg_target[c] == 0) avg_target[c] = 1;
    }

    for (int dy = 0; dy < tile_h; ++dy) {
        for (int dx = 0; dx < tile_w; ++dx) {
            for (int c = 0; c < 3; ++c) {
                int corrected_value = tile_img[dy][dx][c] * avg_target[c] / avg_tile[c];
                tile_img[dy][dx][c] = std::min(255, std::max(0, corrected_value));
            }
        }
    }
}

int*** PhotoMosaic::resizeTile(int*** pixels, int old_width, int old_height, int new_width, int new_height) {
    int*** resized_pixels = new int**[new_height];
    for (int i = 0; i < new_height; ++i) {
        resized_pixels[i] = new int*[new_width];
        for (int j = 0; j < new_width; ++j) {
            resized_pixels[i][j] = new int[3];
            float x_ratio = float(old_width) / new_width;
            float y_ratio = float(old_height) / new_height;
            int x = int(j * x_ratio);
            int y = int(i * y_ratio);
            for (int c = 0; c < 3; ++c) {
                resized_pixels[i][j][c] = pixels[y][x][c];
            }
        }
    }
    return resized_pixels;
}

void PhotoMosaic::freePixels(int*** pixels, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            delete[] pixels[i][j];
        }
        delete[] pixels[i];
    }
    delete[] pixels;
}

