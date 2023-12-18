#pragma once


// Perform erosion on img, with structuring element sel
Mat_<uchar> erosion(
    Mat_<uchar> img,
    Mat_<uchar> sel
);


// Perform dilation on img, with structuring element sel
Mat_<uchar> dilation(
    Mat_<uchar> img,
    Mat_<uchar> sel
);


// Perform opening on img, with structuring element sel
Mat_<uchar> opening(
    Mat_<uchar> img,
    Mat_<uchar> sel
);


// Perform closing on img, with structuring element sel
Mat_<uchar> closing(
    Mat_<uchar> img,
    Mat_<uchar> sel
);