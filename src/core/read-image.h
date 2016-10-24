#ifndef READIMAGE_H
#define READIMAGE_H

#include <iostream>


bool read_exr_rgba(std::string name, float *&rgba, int &xRes, int &yRes, bool hasAlpha=true);
bool read_exr_rgb(const std::string &name, float *&rgba, int &width, int &height);
bool read_exr_grey(std::string name, float *&grey, int &xRes, int &yRes);
bool read_exr_rgb2y(const std::string &name, float *&Y, int &width,int &height);
#endif
