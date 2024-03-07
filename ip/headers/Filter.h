#pragma once


// Performs gaussian filtering with kernel of width w
Mat_<uchar> gaussianFilter(
    Mat_<uchar> img,
    int w
);