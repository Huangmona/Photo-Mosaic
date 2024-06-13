#ifndef IMAGE_H
#define IMAGE_H

#include "data_loader.h"
#include <string>

using namespace std;

class Image {
    protected:
        int w;
        int h;
        static Data_Loader data_loader;

    public:
        Image();
        Image(int width, int height);
        virtual ~Image();

        int get_w();
        int get_h();

        virtual bool LoadImage(string& filename) = 0;
        virtual void DumpImage(string& filename) = 0;
        virtual void Display_ASCII() = 0;
        virtual void Display_CMD() = 0;
};

#endif // IMAGE_H
