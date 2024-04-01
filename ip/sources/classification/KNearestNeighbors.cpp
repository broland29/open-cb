#include "../../headers/classification/KNearestNeighbors.h"


// ---------- public (overriden) functions ---------- //

KNearestNeighbors::KNearestNeighbors(int k)
{
    this->k = k;
}


int KNearestNeighbors::train()
{
    std::vector<std::pair<Mat_<Vec3b>, uchar>> trainImages;
    if (FileHandler::readLabelFolderImages(TRAIN_FOLDER_PATH, trainImages) != 0)
    {
        return 1;
    }

    // --- get X and y --- //
    const int m = 8;        // number of bins in histogram(s)
    const int d = 3 * m;    // number of features, width of X

    // unknown number of images -> unknown number of rows -> start with 0 and push_back
    Mat_<double> X(0, d);  // feature matrix
    Mat_<uchar> y(0, 1);   // class labels (classes from 0 to C-1)

    for (auto const& pair : trainImages)
    {
        Mat_<Vec3b> image = pair.first;  // vector of images
        uchar label = pair.second;

        Mat_<double> feature = getFeatureHistogram(image);
        X.push_back(feature);

        Mat_<uchar> label_(1, 1);
        label_(0, 0) = label;
        y.push_back(label);
    }

    this->X = X.clone();
    this->y = y.clone();


    // --- train --- //
    // KNN does not require training
}


int KNearestNeighbors::test()
{
    std::vector<std::pair<Mat_<Vec3b>, uchar>> testImages;
    if (FileHandler::readLabelFolderImages(TEST_FOLDER_PATH, testImages) != 0)
    {
        return 1;
    }

    SPDLOG_TRACE("First five test labels: {} {} {} {} {}",
        testImages[0].second, testImages[1].second, testImages[2].second, testImages[3].second, testImages[4].second);

    Mat_<double> confusionMatrix(CLASS_COUNT, CLASS_COUNT, 0.0);  // row: predicted class  col: actual class
    for (auto const& pair : testImages)
    {
        Mat_<Vec3b> img = pair.first;		// vector of images
        uchar actualClass = pair.second;	// class
        uchar predictedClass = classify(img);

        confusionMatrix(actualClass, predictedClass)++;  // works since classes encoded 0-13
    }

    double correct = 0;
    double wrong = 0;
    for (int i = 0; i < CLASS_COUNT; i++)
    {
        for (int j = 0; j < CLASS_COUNT; j++)
        {
            if (i == j)
            {
                correct += confusionMatrix(i, j);
            }
            else
            {
                wrong += confusionMatrix(i, j);
            }
        }
    }
    double accuracy = correct / (correct + wrong);

    SPDLOG_INFO("Test image count: {}", testImages.size());

    // trickery: spdlog allows string, Mat can be printed
    SPDLOG_INFO("Confusion matrix:");
    std::string str = "\n";
    str << confusionMatrix;
    SPDLOG_INFO(str);

    SPDLOG_INFO("Accuracy: {}", accuracy);
}


std::string KNearestNeighbors::classifyBoard()
{
    // read cell images
    std::array<std::array<Mat_<Vec3b>, 8>, 8> boardImages;
    if (FileHandler::readBoardImages(boardImages) != 0)
    {
        return "";
    }

    // compose encoding by successive classifications
    std::string boardEncodings(64, ' ');
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            boardEncodings[i * 8 + j] = classify(boardImages[i][j]);
        }
    }

    return boardEncodings;
}


int KNearestNeighbors::save(std::string folderPath)
{
    // todo
    return 404;
}


int KNearestNeighbors::load(std::string folderPath)
{
    // todo
    return 404;
}


// ---------- private (helper) functions ---------- //

uchar KNearestNeighbors::classify(Mat_<Vec3b> image)
{
    Mat_<double> feature = getFeatureHistogram(image);

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
        distancesAndLabels.push_back(distanceAndLabel{ distance, y(i) });
    }

    sort(distancesAndLabels.begin(), distancesAndLabels.end());

    // check k nearest neighbors, each neighbor's label counts as a vote, highest vote wins
    std::vector<int> votes;
    for (int i = 0; i < CLASS_COUNT; i++)
    {
        votes.push_back(0);
    }
    for (int i = 0; i < k; i++)
    {
        votes[distancesAndLabels[i].label]++;  // works, since uchar
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

    return winningLabel;
}


// colorIndex:  0 = blue, 1 = green, 2 = red
// m:           total number of bins
void KNearestNeighbors::getHistogram(Mat_<Vec3b> img, int colorIndex, int* hist)
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
Mat_<double> KNearestNeighbors::getFeatureHistogram(Mat_<Vec3b> img)
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
