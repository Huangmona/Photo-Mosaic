#include "../inc/photo_mosaic.h"
#include <cmath>
#include <cfloat> // For DBL_MAX
#include <iostream>
#include <vector>
#include <algorithm> // For std::min

using namespace std;

PhotoMosaic::PhotoMosaic(const string& target_image_path, const vector<string>& tile_image_paths, int tile_size, Data_Loader& data_loader)
    : tile_size(tile_size), data_loader(data_loader) {

    // 加载目标图像作为 RGB 图像
    target_image_rgb = data_loader.Load_RGB(target_image_path, &width, &height);
    cout << "Loaded target image: " << target_image_path << ", width: " << width << ", height: " << height << endl;

    // 加载瓦片图像
    for (const string& path : tile_image_paths) {
        int tile_w, tile_h;
        int*** tile_img = data_loader.Load_RGB(path, &tile_w, &tile_h);
        // 调整瓦片图像大小以匹配 tile_size
        if (tile_w != tile_size || tile_h != tile_size) {
            int*** resized_tile_img = resizeTile(tile_img, tile_w, tile_h, tile_size, tile_size);
            freePixels(tile_img, tile_w, tile_h);
            tile_img = resized_tile_img;
            tile_w = tile_size;
            tile_h = tile_size;
        }
        tile_images.push_back(tile_img);
        tile_widths.push_back(tile_w);
        tile_heights.push_back(tile_h);
    }
}

PhotoMosaic::~PhotoMosaic() {
    // 释放 target_image_rgb 内存
    freePixels(target_image_rgb, width, height);

    // 释放 tile_images 内存
    for (size_t i = 0; i < tile_images.size(); ++i) {
        freePixels(tile_images[i], tile_widths[i], tile_heights[i]);
    }
}

void PhotoMosaic::createMosaic(const string& output_path) {
    // 创建马赛克图像
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
        int tile_w = tile_widths[i];
        int tile_h = tile_heights[i];

        // 从目标图像中提取 tile_size 的部分进行比较
        double diff = calculateDiff(tile_img, target_image_rgb, tile_w, tile_h, x, y, tile_size, tile_size);
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
    // 计算瓦片图像的颜色均值
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
        if (avg_tile[c] == 0) avg_tile[c] = 1; // 防止除以零
    }

    // 计算目标图像的颜色均值
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
        if (avg_target[c] == 0) avg_target[c] = 1; // 防止除以零
    }

    // 颜色校正
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
