#pragma once

#include "../CommonIP.h"
#include "Conversion.h"


// Performs gaussian filtering with kernel of width w
Mat_<uchar> gaussianFilter(
    Mat_<uchar> img,
    int w
);


// Performs convolution on img with kernel H
Mat_<float> convolution(
    Mat_<uchar> img,
    Mat_<float> H,
    uchar padding = 255
);