#include "write-exr.h"

#include <ImfRgba.h>
#include <ImfRgbaFile.h>

using namespace Imf;
using namespace Imath;


template <typename T>
void write_exr_grey(std::string name, const T* pixels, int xRes, int yRes){

    Rgba *hrgba = new Rgba[xRes * yRes]();

    for(int r =0; r < yRes; r++)
        for(int c = 0; c < xRes; c++)
            hrgba[r*xRes+c] = Rgba(pixels[(r)*xRes+c], pixels[(r)*xRes+c],pixels[(r)*xRes+c], 1);

    Box2i displayWindow(V2i(0,0), V2i(xRes-1, yRes-1));
    Box2i dataWindow = displayWindow;

    if(name.compare(name.size()-4, 4,".exr") != 0){
        name.erase(name.end()-4, name.end());
        name += ".exr";
    }

    RgbaOutputFile file(name.c_str(), displayWindow, dataWindow, WRITE_RGBA);
    file.setFrameBuffer(hrgba, 1, xRes);
    try {
        file.writePixels(yRes);
    }
    catch (const std::exception &e) {
        fprintf(stderr, "Unable to write image file \"%s\": %s", name.c_str(),
                e.what());
    }

    delete[] hrgba;

}

template <typename T>
void write_exr_rgb(std::string name, const T* pixels, int xRes, int yRes){

    Rgba *hrgba = new Rgba[xRes * yRes]();

    //Problem: causes vertical flip of the data (image)
    //for (int i = 0; i < xRes * yRes; ++i)
    //hrgba[i] = Rgba(pixels[4*i], pixels[4*i+1], pixels[4*i+2], 1.);

    for(int r = 0; r < yRes; r++)
        for(int c = 0; c < xRes; c++)
            hrgba[r*xRes+c] = Rgba(pixels[3*(r*xRes+c)+0], pixels[3*(r*xRes+c)+1], pixels[3*(r*xRes+c)+2], 1);

    Box2i displayWindow(V2i(0,0), V2i(xRes-1, yRes-1));
    Box2i dataWindow = displayWindow;

    if(name.compare(name.size()-4, 4,".exr") != 0){
        name.erase(name.end()-4, name.end());
        name += ".exr";
    }

    RgbaOutputFile file(name.c_str(), displayWindow, dataWindow, WRITE_RGBA);
    file.setFrameBuffer(hrgba, 1, xRes);
    try {
        file.writePixels(yRes);
    }
    catch (const std::exception &e) {
        fprintf(stderr, "Unable to write image file \"%s\": %s", name.c_str(),
                e.what());
    }

    delete[] hrgba;
}

template <typename T>
void write_exr_rgba(std::string name, const T* pixels, int xRes, int yRes){

    Rgba *hrgba = new Rgba[xRes * yRes]();

    //Problem: causes vertical flip of the data (image)
    //for (int i = 0; i < xRes * yRes; ++i)
    //hrgba[i] = Rgba(pixels[4*i], pixels[4*i+1], pixels[4*i+2], 1.);

    for(int r = 0; r < yRes; r++)
        for(int c = 0; c < xRes; c++)
            hrgba[r*xRes+c] = Rgba(pixels[4*(r*xRes+c)+0], pixels[4*(r*xRes+c)+1], pixels[4*(r*xRes+c)+2], pixels[4*(r*xRes+c)+3]);

    Box2i displayWindow(V2i(0,0), V2i(xRes-1, yRes-1));
    Box2i dataWindow = displayWindow;

    if(name.compare(name.size()-4, 4,".exr") != 0){
        name.erase(name.end()-4, name.end());
        name += ".exr";
    }

    RgbaOutputFile file(name.c_str(), displayWindow, dataWindow, WRITE_RGBA);
    file.setFrameBuffer(hrgba, 1, xRes);
    try {
        file.writePixels(yRes);
    }
    catch (const std::exception &e) {
        fprintf(stderr, "Unable to write image file \"%s\": %s", name.c_str(),
                e.what());
    }

    delete[] hrgba;
}

template void write_exr_grey(std::string name, const float *pixels, int xRes, int yRes);
template void write_exr_rgb(std::string name, const float *pixels, int xRes, int yRes);
template void write_exr_rgba(std::string name, const float *pixels, int xRes, int yRes);
<<<<<<< HEAD
//template void write_exr_float(std::string name, const float *pixels, int xRes, int yRes);
=======
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
