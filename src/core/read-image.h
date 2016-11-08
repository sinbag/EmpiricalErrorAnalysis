#ifndef READIMAGE_H
#define READIMAGE_H

#include <iostream>
<<<<<<< HEAD
#include <FreeImage.h>
=======
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb


bool read_exr_rgba(std::string name, float *&rgba, int &xRes, int &yRes, bool hasAlpha=true);
bool read_exr_rgb(const std::string &name, float *&rgba, int &width, int &height);
bool read_exr_grey(std::string name, float *&grey, int &xRes, int &yRes);
bool read_exr_rgb2y(const std::string &name, float *&Y, int &width,int &height);
<<<<<<< HEAD
float *readMultiChannelEXR(std::string fileName, int *width, int *height, int *nbins);
=======
>>>>>>> ef97f5bea24b64a17cb0751ce4798e13ff2573bb
#endif
