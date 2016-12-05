#include <stdio.h>
#include <stdlib.h>
#include <ImfInputFile.h>
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <ImfFrameBuffer.h>
#include <ImfRgbaFile.h>
#include <half.h>
#include <assert.h>
#include <vector>

using namespace Imf;
using namespace Imath;

#include "read-image.h"

//Adapted from PBRT2-src code
bool read_exr_grey(std::string name, float *&grey, int &xRes, int &yRes){
    try {
        InputFile file(name.c_str());
        Box2i dw = file.header().dataWindow();
        xRes = dw.max.x - dw.min.x + 1;
        yRes = dw.max.y - dw.min.y + 1;

        half *hgrey = new half[xRes * yRes];

        int nChannels = 1;

        hgrey -= (dw.min.x + dw.min.y * xRes);
        FrameBuffer frameBuffer;
        frameBuffer.insert("R", Slice(HALF, (char *)hgrey, sizeof(half), xRes * sizeof(half), 1, 1, 0.0));

        file.setFrameBuffer(frameBuffer);
        file.readPixels(dw.min.y, dw.max.y);

        hgrey += (dw.min.x + dw.min.y * xRes);
        grey = new float[nChannels * xRes * yRes];
        for (int i = 0; i < nChannels * xRes * yRes; ++i)
        grey[i] = hgrey[i];
        delete[] hgrey;
    } catch (const std::exception &e) {
        fprintf(stderr, "Unable to read image file \"%s\": %s", name.c_str(), e.what());
        return false;
    }

    return grey;
}

bool read_exr_rgb(const std::string &name, float *&rgba, int &width,
                                 int &height) {
    using namespace Imf;
    using namespace Imath;
    try {
        RgbaInputFile file(name.c_str());
        Box2i dw = file.dataWindow();
        width = dw.max.x - dw.min.x + 1;
        height = dw.max.y - dw.min.y + 1;
        std::vector<Rgba> pixels(width * height);

        file.setFrameBuffer(&pixels[0] - dw.min.x - dw.min.y * width, 1,
                            width);
        file.readPixels(dw.min.y, dw.max.y);

        rgba = new float[3* width* height]();
        for (int i = 0; i < width * height; ++i) {
            rgba[3*i+0] = pixels[i].r;
            rgba[3*i+1] = pixels[i].g;
            rgba[3*i+2] = pixels[i].b;
        }
        return true;
    } catch (const std::exception &e) {
        fprintf(stderr, "Unable to read image file \"%s\": %s", name.c_str(), e.what());
    }

    return false;
}


//Adapted from PBRT2-src code
bool read_exr_rgba(std::string name, float *&rgba, int &xRes, int &yRes, bool hasAlpha){
    try {
        InputFile file(name.c_str());
        Box2i dw = file.header().dataWindow();
        xRes = dw.max.x - dw.min.x + 1;
        yRes = dw.max.y - dw.min.y + 1;
        // for now...
        int nChannels = 3;

        if(hasAlpha)
            nChannels = 4;

        half *hrgba = new half[nChannels * xRes * yRes];

        hrgba -= nChannels * (dw.min.x + dw.min.y * xRes);
        FrameBuffer frameBuffer;
        frameBuffer.insert("R", Slice(HALF, (char *)hrgba,
                                      nChannels*sizeof(half), xRes * nChannels * sizeof(half), 1, 1, 0.0));
        frameBuffer.insert("G", Slice(HALF, (char *)hrgba+sizeof(half),
                                      nChannels*sizeof(half), xRes * nChannels * sizeof(half), 1, 1, 0.0));
        frameBuffer.insert("B", Slice(HALF, (char *)hrgba+2*sizeof(half),
                                      nChannels*sizeof(half), xRes * nChannels * sizeof(half), 1, 1, 0.0));
        if(hasAlpha){
            frameBuffer.insert("A", Slice(HALF, (char *)hrgba+3*sizeof(half),
                                      nChannels*sizeof(half), xRes * nChannels * sizeof(half), 1, 1, 1.0));
        }

        file.setFrameBuffer(frameBuffer);
        file.readPixels(dw.min.y, dw.max.y);

        hrgba += nChannels * (dw.min.x + dw.min.y * xRes);
        rgba = new float[nChannels * xRes * yRes];
        for (int i = 0; i < nChannels * xRes * yRes; ++i)
        rgba[i] = hrgba[i];
        delete[] hrgba;
    } catch (const std::exception &e) {
        fprintf(stderr, "Unable to read image file \"%s\": %s", name.c_str(), e.what());
        return false;
    }

    return rgba;
}

bool read_exr_rgb2y(const std::string &name, float *&Y, int &width,int &height) {

    using namespace Imf;
    using namespace Imath;
    try {
        RgbaInputFile file(name.c_str());
        Box2i dw = file.dataWindow();
        width = dw.max.x - dw.min.x + 1;
        height = dw.max.y - dw.min.y + 1;
        std::vector<Rgba> pixels(width * height);

        file.setFrameBuffer(&pixels[0] - dw.min.x - dw.min.y * width, 1,
                            width);
        file.readPixels(dw.min.y, dw.max.y);

        Y = new float[width* height]();
        for (int i = 0; i < width * height; ++i) {
            Y[i] = 0.299   *pixels[i].r		+ 0.587  *pixels[i].g		+ 0.114  *pixels[i].b;
        }

        return true;
    } catch (const std::exception &e) {
        fprintf(stderr, "Unable to read image file \"%s\": %s", name.c_str(), e.what());
    }

    return false;
}

