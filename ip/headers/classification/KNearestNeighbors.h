#pragma once

#include "CxxClassifier.h"
#include "../file_handling/FileHandler.h"
#include "../Common.h"
#include "../Parameters.h"
#include <fstream>


class KNearestNeighbors : public CxxClassifier
{
public:
    int k;              // number of neighbors
    int numberOfBins;

private:
    Mat_<int> X;        // feature matrix (one row = one feature = one fixed-size color histogram)
    Mat_<int> y;        // class labels (0, 1, ..., see internalToExternal, externalToInternal)

public:
    KNearestNeighbors(KNearestNeighborsParameters kNearestNeighborsParameters);

    int train() override;

    int test() override;

    int classifyBoard(QVector<QString>& encodings) override;

    int save() override;

    int load() override;

private:
    // sturct for one potential vote
    struct distanceAndLabel
    {
        double distance;
        int label;

        // override comparison so std::sort can be used
        bool operator < (const distanceAndLabel& o) const {
            return distance < o.distance;  // we will sort increasing!
        }
    };

    // return one histogram
    //  colorIndex:  0 = blue, 1 = green, 2 = red
    //  m:           total number of bins
    void getHistogram(Mat_<Vec3b> img, int colorIndex, Mat_<int> hist);
    
    // return 3 color histograms "in a row" (basically image -> feature)
    Mat_<int> getFeatureHistogram(Mat_<Vec3b> img);

    // return the inferred label of image
    int classify(Mat_<Vec3b> image);

    // helper function to log distribution of samples
    void logImagesDistribution(std::vector<std::pair<Mat_<Vec3b>, QString>> images, std::string imageType);

    void logExampleFeatures();
};
