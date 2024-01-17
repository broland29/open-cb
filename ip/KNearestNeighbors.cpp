#include "stdafx.h"
#include "KNearestNeighbors.h"

// colorIndex:  0 = blue, 1 = green, 2 = red
// m:           total number of bins
void getHistogram(Mat_<Vec3b> img, int colorIndex, int* hist)
{
    for (int i = 0; i < KNN_TOTAL_NO_OF_BINS; i++)
    {
        hist[i] = 0;
    }

    int const binSize = 256 / KNN_TOTAL_NO_OF_BINS;
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            hist[img.at<Vec3b>(i, j)[colorIndex] / binSize] ++;
        }
    }
}

// returns 3 color histograms "in a row" (like in feature matrix)
Mat_<double> getFeatureKNearest(Mat_<Vec3b> img)
{
    Mat_<double> feature(1, KNN_TOTAL_NO_OF_BINS * 3, CV_32FC1);

    for (int i = 0; i < 3; i++)  // colors
    {
        int histogram[KNN_TOTAL_NO_OF_BINS];
        getHistogram(img, i, histogram);
        for (int j = 0; j < KNN_TOTAL_NO_OF_BINS; j++)
        {
            feature(0, i * KNN_TOTAL_NO_OF_BINS + j) = histogram[j];
        }
    }

    return feature;
}

struct distanceAndLabel
{
    float distance;
    int label;

    // override comparison so std::sort can be used
    bool operator < (const distanceAndLabel& o) const {
        return distance < o.distance;  // we will sort increasing!
    }
};

std::string KNearestNeighborsClassifier::classify(Mat_<Vec3b> image)
{
    Mat_<double> feature = getFeatureKNearest(image);

    // calculate distance from each point and their labels
    std::vector<distanceAndLabel> distancesAndLabels;
    float distance = 0;
    for (int i = 0; i < X.rows; i++)  // images
    {
        for (int j = 0; j < X.cols; j++)  // features
        {
            distance +=
                (feature(0, j) - X(i, j)) *
                (feature(0, j) - X(i, j));
        }
        distance = sqrt(distance);
        distancesAndLabels.push_back(distanceAndLabel{ distance, y.at<uchar>(i) });
    }

    sort(distancesAndLabels.begin(), distancesAndLabels.end());

    // check k nearest neighbors, each neighbor's label counts as a vote, highest vote wins
    std::vector<int> votes;
    for (int i = 0; i < classEncodings.size(); i++)
    {
        votes.push_back(0);
    }
    for (int i = 0; i < k; i++)
    {
        votes[distancesAndLabels[i].label]++;
    }
    int maxVotes = votes[0];
    int winningLabel = 0;
    for (int i = 1; i < k; i++)
    {
        if (votes[i] > maxVotes)
        {
            maxVotes = votes[i];
            winningLabel = i;
        }
    }

    return classEncodings[winningLabel];
}

KNearestNeighborsClassifier::KNearestNeighborsClassifier(std::map<std::string, std::vector<Mat_<Vec3b>>> trainImages, int k)
{
    const int m = 8;        // number of bins in histogram(s)
    const int d = 3 * m;    // number of features, width of X

    // unknown number of images -> unknown number of rows -> start with 0 and push_back
    Mat_<double> X(0, d);  // feature matrix
    Mat_<uchar> y(0, 1);   // class labels (classes from 0 to C-1)

    std::map<uchar, std::string> classEncodings;
    uchar classEncoding = 0;

    for (auto const& pair : trainImages)
    {
        std::string classString = pair.first;           // class
        std::vector<Mat_<Vec3b>> images = pair.second;  // vector of images
        classEncodings.insert(std::pair<uchar, std::string>(classEncoding, classString));

        for (Mat_<Vec3b> image : images)
        {
            Mat_<double> feature = getFeatureKNearest(image);
            X.push_back(feature);

            Mat_<uchar> label(1, 1);
            label.at<uchar>(0, 0) = classEncoding;
            y.push_back(label);
        }

        classEncoding++;
    }

    this->X = X.clone();
    this->y = y.clone();
    this->k = k;
    this->classEncodings = std::map<uchar, std::string>(classEncodings);
}
