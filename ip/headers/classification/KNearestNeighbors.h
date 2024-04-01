#pragma once

#include "../Common.h"
#include "../EncodingMapperIP.h"

#include "AbstractClassifier.h"
#include "../file_handling/FileHandler.h"


#define KNN_TOTAL_NO_OF_BINS 8

class KNearestNeighbors : public AbstractClassifier
{
public:
    
private:
    Mat_<double> X;     // feature matrix (one row = one feature = one fixed-size color histogram)
    Mat_<uchar> y;      // class labels (see EncodingMapperIP)
    int k;              // number of neighbors

public:
    KNearestNeighbors(int k = 5);

    virtual int train() override;

    virtual int test() override;

    virtual std::string classifyBoard() override;

    virtual int save(std::string folderPath) override;

    virtual int load(std::string folderPath) override;

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

    uchar classify(Mat_<Vec3b> image);
};
