#ifndef _PHOTO_MOSAIC_H_
#define _PHOTO_MOSAIC_H_

#include <vector>
#include <string>
#include "CImg.h"
using namespace cimg_library;

class PhotoMosaic {
public:
    void createMosaic(const std::vector<std::string>& imageFilenames, const std::string& outputFilename);
};

#endif
