#pragma once

#include "../Util.h"
#include "Classifier.h"
/*
#define KNN_TOTAL_NO_OF_BINS 8

class KNearestNeighborsClassifier : public Classifier
{
public:
    Mat_<double> X;  // feature matrix
    Mat_<Piece> y;   // class labels (classes from 0 to C-1)
    int k;

    KNearestNeighborsClassifier(int k);

    void train(std::vector<std::pair<Mat_<Vec3b>, Piece>> trainImages);

    Piece classify(Mat_<Vec3b> image);

    void test(std::vector<Mat_<Vec3b>> testImages);

private:
    void getHistogram(Mat_<Vec3b> img, int colorIndex, int* hist);
    Mat_<double> getFeatureHistogram(Mat_<Vec3b> img);

    struct distanceAndLabel
    {
        float distance;
        Piece label;

        // override comparison so std::sort can be used
        bool operator < (const distanceAndLabel& o) const {
            return distance < o.distance;  // we will sort increasing!
        }
    };
};
*/