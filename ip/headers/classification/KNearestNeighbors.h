#pragma once

#include "AbstractClassifier.h"
#include "../file_handling/FileHandler.h"
#include "../Common.h"
#include <fstream>


#define KNN_K 5
#define KNN_TOTAL_NO_OF_BINS 8
#define KNN_DEBUG true
#define KNN_UNITE_FREES true

class KNearestNeighbors : public AbstractClassifier
{
public:
    
private:
    bool trained;       // true if train was ran at least once

    Mat_<int> X;        // feature matrix (one row = one feature = one fixed-size color histogram)
    Mat_<int> y;        // class labels (0, 1, ..., see internalToExternal, externalToInternal)
    int k;              // number of neighbors

public:
    KNearestNeighbors();

    int train() override;

    int test() override;

    int classifyBoard(QVector<QString>& encodings) override;

    int save() override;

    int load() override;

    static void calculateAndLogMetrics(std::vector<std::vector<int>> confusionMatrix, int testSize);
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
    void getHistogram(Mat_<Vec3b> img, int colorIndex, int* hist);
    
    // return 3 color histograms "in a row" (basically image -> feature)
    Mat_<int> getFeatureHistogram(Mat_<Vec3b> img);

    // return the inferred label of image
    int classify(Mat_<Vec3b> image);

    // map external encoding (see ENCODINGS in Common.h) to internal (0, 1, ...)
    int externalToInternal(QString encoding);

    // map internal encoding (0, 1, ...) to external (see ENCODINGS in Common.h)
    QString internalToExternal(int encoding);

    // helper function to log distribution of samples
    void logImagesDistribution(std::vector<std::pair<Mat_<Vec3b>, QString>> images, std::string imageType);

    void logExampleFeatures();

    std::vector<std::pair<Mat_<Vec3b>, QString>> uniteFrees(std::vector<std::pair<Mat_<Vec3b>, QString>> images);
};
