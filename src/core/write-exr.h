#ifndef WRITE_EXR_H
#define WRITE_EXR_H
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>

template <typename T>
void write_exr_grey(std::string name, const T*  pixels, int xRes, int yRes);
template <typename T>
void write_exr_rgb(std::string name, const T*  pixels, int xRes, int yRes);

template <typename T>
void write_exr_rgba(std::string name, const T*  pixels, int xRes, int yRes);

#endif
