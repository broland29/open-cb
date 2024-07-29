#pragma once

#include "CxxClassifier.h"


class KNearestNeighbors : public CxxClassifier
{
public:
    int k;              // parameter
    int numberOfBins;   // parameter

private:
    Mat_<int> X;        // feature matrix (one row = one feature = one fixed-size color histogram)
    Mat_<int> y;        // class labels (see CxxClassifier::internalToExternal, CxxClassifier::externalToInternal)

public:
    KNearestNeighbors(KNearestNeighborsParameters kNearestNeighborsParameters);

    int train() override;

    int test() override;

    int classifyBoard(QVector<QString>& encodings) override;

    int save() override;

    int load() override;

private:
    // struct for one potential vote
    struct distanceAndLabel
    {
        double distance;
        int label;

        // override comparison so std::sort can be used
        bool operator < (const distanceAndLabel& o) const {
            return distance < o.distance;  // we will sort increasing!
        }
    };

    // return one histogram (related to one color channel)
    void getHistogram(
        Mat_<Vec3b> img,            // original image
        int colorIndex,             // 0 = blue, 1 = green, 2 = red
        Mat_<int> hist              // resulting histogram
    );
    
    // return 3 color histograms "in a row" (basically image -> feature)
    Mat_<int> getFeatureHistogram(
        Mat_<Vec3b> img             // original image
    );

    // return the inferred label of image
    int classify(
        Mat_<Vec3b> image           // original image
    );

    // log a few features as illustration
    void logExampleFeatures();
};
