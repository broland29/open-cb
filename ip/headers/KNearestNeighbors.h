#pragma once

#include "Util.h"

#define KNN_TOTAL_NO_OF_BINS 8

struct KNearestNeighborsClassifier
{
    Mat_<double> X;  // feature matrix
    Mat_<uchar> y;   // class labels (classes from 0 to C-1)
    int k;
    std::map<uchar, std::string> classEncodings;

    KNearestNeighborsClassifier(std::map<std::string, std::vector<Mat_<Vec3b>>> trainImages, int k);

    std::string classify(Mat_<Vec3b> image);
};