#pragma once

#include "../Util.h"
#include "AbstractClassifier.h"

#define KNN_TOTAL_NO_OF_BINS 8

class KNearestNeighborsClassifier : public AbstractClassifier
{
public:
    
private:
    Mat_<double> X;     // feature matrix (currently pixels only)
    Mat_<uchar> y;      // class labels (see EncodingMapperIP)
    int k;              // number of neighbors

public:
    KNearestNeighborsClassifier(int k);

    void train(std::vector<std::pair<Mat_<Vec3b>, uchar>> trainImages);

    uchar classify(Mat_<Vec3b> image);

private:
    struct distanceAndLabel
    {
        float distance;
        uchar label;

        // override comparison so std::sort can be used
        bool operator < (const distanceAndLabel& o) const {
            return distance < o.distance;  // we will sort increasing!
        }
    };

    void getHistogram(Mat_<Vec3b> img, int colorIndex, int* hist);
    
    Mat_<double> getFeatureHistogram(Mat_<Vec3b> img);
};
