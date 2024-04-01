#pragma once

#include "../Common.h"


// Converts grayscale image img to binary based on threshold
Mat_<uchar> grayscaleToBinary(
    Mat_<uchar> img,
    uchar threshold
);


// Converts binary image img to color
Mat_<Vec3b> binaryToColor(
    Mat_<uchar>
    img
);


// Converts float image img (result of a convolution) to a grayscale image based
// on the kernel used to get img. Basically performs normalization
Mat_<uchar> floatToGrayscale(
    Mat_<float> convImg,
    Mat_<float> H
);