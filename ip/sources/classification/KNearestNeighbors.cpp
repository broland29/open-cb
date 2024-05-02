#include "../../headers/classification/KNearestNeighbors.h"


KNearestNeighbors::KNearestNeighbors()
{
    this->k = KNN_K;
    trained = false;
}


int KNearestNeighbors::train()
{
    SPDLOG_TRACE("Starting training");
    
    std::vector<std::pair<Mat_<Vec3b>, QString>> _trainImages;
    if (FileHandler::readLabelFolderImages(TRAIN_FOLDER_PATH, _trainImages) != 0)
    {
        return 1;
    }

    // since validation not used in other ways, for KNN I use those images as train images as well
    std::vector<std::pair<Mat_<Vec3b>, QString>> _validationImages;
    if (FileHandler::readLabelFolderImages(VALIDATION_FOLDER_PATH, _validationImages) != 0)
    {
        return 2;
    }
    _trainImages.insert(_trainImages.end(), _validationImages.begin(), _validationImages.end());

    // for KNN with color histograms, having separate WF and BF creates confusion
    std::vector<std::pair<Mat_<Vec3b>, QString>> trainImages;
    if (KNN_UNITE_FREES)
    {
        trainImages = uniteFrees(_trainImages);
    }
    else
    {
        trainImages = _trainImages;
    }

    if (KNN_DEBUG)
    {
        logImagesDistribution(trainImages, "train");
    }

    const int m = KNN_TOTAL_NO_OF_BINS;     // number of bins in histogram(s)
    const int d = 3 * m;                    // number of features, width of X

    // unknown number of images -> unknown number of rows -> start with 0 and push_back
    Mat_<int> X(0, d);  // feature matrix
    Mat_<int> y(0, 1);   // class labels

    for (auto const& pair : trainImages)
    {
        Mat_<Vec3b> image = pair.first;
        int label = externalToInternal(pair.second);

        Mat_<int> feature = getFeatureHistogram(image);
        X.push_back(feature);

        Mat_<int> label_(1, 1);
        label_(0, 0) = label;
        y.push_back(label);
    }

    // store X and y, since will need it later
    this->X = X.clone();
    this->y = y.clone();

    if (KNN_DEBUG)
    {
        SPDLOG_TRACE("Built {} features", X.rows);
        logExampleFeatures();
    }

    // KNN does not require "actual training"
    trained = true;

    SPDLOG_TRACE("Finished training");
    return 0;
}


int KNearestNeighbors::test()
{
    if (!trained)
    {
        SPDLOG_ERROR("Should train first");
        return 1;
    }

    std::vector<std::pair<Mat_<Vec3b>, QString>> _testImages;
    if (FileHandler::readLabelFolderImages(TEST_FOLDER_PATH, _testImages) != 0)
    {
        return 2;
    }

    // for KNN with color histograms, having separate WF and BF creates confusion
    std::vector<std::pair<Mat_<Vec3b>, QString>> testImages;
    int classCount;
    if (KNN_UNITE_FREES)
    {
        testImages = uniteFrees(_testImages);
        classCount = ENCODINGS.size() - 1;
    }
    else
    {
        testImages = _testImages;
        classCount = ENCODINGS.size();
    }

    if (KNN_DEBUG)
    {
        logImagesDistribution(testImages, "test");
    }

    // confustion matrix: on x axis we have predicted class, on y we have actual class
    std::vector<std::vector<int>> confusionMatrix;

    confusionMatrix.resize(classCount);
    for (int i = 0; i < classCount; i++)
    {
        confusionMatrix[i].resize(classCount, 0);
    }

    const int testSize = testImages.size();
    for (int i = 0; i < testSize; i++)
    {
        Mat_<Vec3b> img = testImages[i].first;
        int actualClass = externalToInternal(testImages[i].second);	// class
        int predictedClass = classify(img);

        confusionMatrix[actualClass][predictedClass]++;  // using class directly to access slot!

        // just to keep the console interactive
        if (i % 100 == 0)
        {
            SPDLOG_TRACE("{}/{}", i, testSize);
        }
    }

    calculateAndLogMetrics(confusionMatrix, testSize);

    return 0;
}


int KNearestNeighbors::classifyBoard(QVector<QString>& encodings)
{
    if (!trained)
    {
        SPDLOG_ERROR("Should train first");
        return 1;
    }

    // read cell images
    std::array<std::array<Mat_<Vec3b>, 8>, 8> boardImages;
    if (FileHandler::readBoardImages(boardImages) != 0)
    {
        return 2;
    }

    // compose encodings by successive classifications
    encodings.resize(64);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            encodings[i * 8 + j] = internalToExternal(classify(boardImages[i][j]));
        }
    }

    return 0;
}


int KNearestNeighbors::save()
{
    if (!trained)
    {
        SPDLOG_ERROR("Should train first");
        return 1;
    }

    std::string path = KNN_FOLDER_PATH + std::string("\\knn.txt");
    std::ofstream outfile(path);
    if (!outfile.is_open())
    {
        SPDLOG_ERROR("Could not open {}", path);
        return 2;
    }

    outfile << X.rows << " " << X.cols << "\n";
    for (int i = 0; i < X.rows; i++)
    {
        for (int j = 0; j < X.cols; j++)
        {
            outfile << X(i, j) << " ";
        }
        outfile << "\n";
    }

    outfile << "\n";
    
    outfile << y.rows << " " << y.cols << "\n";
    for (int i = 0; i < y.rows; i++)
    {
        for (int j = 0; j < y.cols; j++)
        {
            outfile << y(i, j) << " ";
        }
        outfile << "\n";
    }

    SPDLOG_TRACE("Saved {} features", X.rows);
    logExampleFeatures();

    return 0;
}


int KNearestNeighbors::load()
{
    std::string path = KNN_FOLDER_PATH + std::string("\\knn.txt");
    std::ifstream infile(path);
    if (!infile.is_open())
    {
        SPDLOG_ERROR("Could not open {}", path);
        return 1;
    }

    int rows, cols;
    
    infile >> rows >> cols;
    Mat_<int> _X(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            infile >> _X(i, j);
        }
    }

    infile >> rows >> cols;
    Mat_<int> _y(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            infile >> _y(i, j);
        }
    }

    X = _X.clone();
    y = _y.clone();
    trained = true;  // since saving only works if trained

    SPDLOG_TRACE("Loaded {} features", X.rows);
    logExampleFeatures();

    return 0;
}


// ---------- private (helper) functions ---------- //

int KNearestNeighbors::classify(Mat_<Vec3b> image)
{
    Mat_<int> feature = getFeatureHistogram(image);

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
    for (int i = 0; i < ENCODINGS.size(); i++)
    {
        votes.push_back(0);
    }
    for (int i = 0; i < k; i++)
    {
        votes[distancesAndLabels[i].label]++;  // using label as index!
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



Mat_<int> KNearestNeighbors::getFeatureHistogram(Mat_<Vec3b> img)
{
    Mat_<int> feature(1, KNN_TOTAL_NO_OF_BINS * 3);

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


int KNearestNeighbors::externalToInternal(QString encoding)
{
    if (!KNN_UNITE_FREES)  // if frees not united, encoding is straight forward
    {
        for (int i = 0; i < ENCODINGS.size(); i++)
        {
            if (encoding == ENCODINGS[i])
            {
                return i;
            }
        }
    }
    else
    {
        if (encoding == "WF") { return 0; };
        if (encoding == "WP") { return 1; };
        if (encoding == "WB") { return 2; };
        if (encoding == "WN") { return 3; };
        if (encoding == "WR") { return 4; };
        if (encoding == "WQ") { return 5; };
        if (encoding == "WK") { return 6; };
        if (encoding == "BF") { return 0; };  // BF interpreted as WF
        if (encoding == "BP") { return 7; };
        if (encoding == "BB") { return 8; };
        if (encoding == "BN") { return 9; };
        if (encoding == "BR") { return 10; };
        if (encoding == "BQ") { return 11; };
        if (encoding == "BK") { return 12; };
    }
    
    SPDLOG_ERROR("Could not convert {}", encoding.toStdString());
    return 0;
}


QString KNearestNeighbors::internalToExternal(int encoding)
{
    if (!KNN_UNITE_FREES)  // if frees not united, encoding is straight forward
    {
        if (encoding < 0 || encoding >= ENCODINGS.size())
        {
            SPDLOG_ERROR("Could not convert {}", encoding);
            return ENCODINGS[0];
        }
        return ENCODINGS[encoding];
    }
    else
    {
        if (encoding == 0) { return "WF"; };  // every free interpreted as WF
        if (encoding == 1) { return "WP"; };
        if (encoding == 2) { return "WB"; };
        if (encoding == 3) { return "WN"; };
        if (encoding == 4) { return "WR"; };
        if (encoding == 5) { return "WQ"; };
        if (encoding == 6) { return "WK"; };
        if (encoding == 7) { return "BP"; };
        if (encoding == 8) { return "BB"; };
        if (encoding == 9) { return "BN"; };
        if (encoding == 10) { return "BR";};
        if (encoding == 11) { return "BQ"; };
        if (encoding == 12) { return "BK"; };

        SPDLOG_ERROR("Could not convert {}", encoding);
        return ENCODINGS[0];
    }
}


void KNearestNeighbors::logImagesDistribution(std::vector<std::pair<Mat_<Vec3b>, QString>> images, std::string imageType)
{
    SPDLOG_TRACE("Loaded {} {} images, out of which:", images.size(), imageType);
    std::vector<int> counts;
    counts.resize(ENCODINGS.size(), 0);
    for (auto const& pair : images)
    {
        counts[externalToInternal(pair.second)]++;
    }
    for (int i = 0; i < counts.size(); i++)
    {
        SPDLOG_TRACE("{} of class {}", counts[i], i);
    }
}


void KNearestNeighbors::logExampleFeatures()
{
    SPDLOG_TRACE("Example features:");
    int indices[] = { 0, X.rows / 2, X.rows - 1 };
    for (int index : indices)
    {
        std::string _index = std::to_string(index);
        std::string _feature;
        _feature << this->X.row(index);
        std::string _label = std::to_string(this->y(index, 0));
        SPDLOG_TRACE("Sample {}:\n\tfeature: {}\n\tlabel: {}", _index, _feature, _label);
    }
}


void KNearestNeighbors::calculateAndLogMetrics(std::vector<std::vector<int>> confusionMatrix, int testSize)
{
    /* Multiclass classification metrics https://www.evidentlyai.com/classification-metrics/multi-class-metrics
    Global metrics:
    Accuracy: correct predictions / all predictions

    Per class metrics:
    Precision:  correctly classified as C / all classified as C
    Recall:     correctly classified as C / all instances of C

    Can traverse confusion matrix "on the axis", since precision i needs row i, recall i needs column i (and accuracy "needs everything")
    */
    const int classCount = confusionMatrix.size();
    std::vector<std::string> precisions(classCount);
    std::vector<std::string> recalls(classCount);
    int totalCorrect = 0;

    for (int i = 0; i < classCount; i++)
    {
        int rowSum = 0;  // sum of elements on row i
        int colSum = 0;  // sum of elements on col i
        for (int j = 0; j < classCount; j++)
        {
            rowSum += confusionMatrix[i][j];
            colSum += confusionMatrix[j][i];
        }

        if (colSum == 0)
        {
            precisions[i] = "No correct classifications of class " + std::to_string(i) +
                " (" + std::to_string(confusionMatrix[i][i]) + "/" + std::to_string(colSum) + ")";
        }
        else
        {
            precisions[i] = std::to_string((double)confusionMatrix[i][i] / colSum) +
                " (" + std::to_string(confusionMatrix[i][i]) + "/" + std::to_string(colSum) + ")";
        }
        if (rowSum == 0)
        {
            recalls[i] = "No test images of class " + std::to_string(i) +
                " (" + std::to_string(confusionMatrix[i][i]) + "/" + std::to_string(rowSum) + ")";
        }
        else
        {
            recalls[i] = std::to_string((double)confusionMatrix[i][i] / rowSum) +
                " (" + std::to_string(confusionMatrix[i][i]) + "/" + std::to_string(rowSum) + ")";
        }
        totalCorrect += confusionMatrix[i][i];
    }
    double accuracy = (double)totalCorrect / testSize;

    // get string version to log
    std::string confusionMatrixString;
    for (int i = 0; i < classCount; i++)
    {
        for (int j = 0; j < classCount; j++)
        {
            confusionMatrixString += std::to_string(confusionMatrix[i][j]) + "\t";
        }
        confusionMatrixString += "\n";
    }

    SPDLOG_INFO("Confusion matrix:\n{}", confusionMatrixString);
    for (int i = 0; i < classCount; i++)
    {
        SPDLOG_TRACE("Class {}:\n\tPrecision: {}\n\tRecall: {}", i, precisions[i], recalls[i]);
    }
    SPDLOG_INFO("Accuracy: {}", accuracy);
}


std::vector<std::pair<Mat_<Vec3b>, QString>> KNearestNeighbors::uniteFrees(std::vector<std::pair<Mat_<Vec3b>, QString>> images)
{
    std::vector<std::pair<Mat_<Vec3b>, QString>> _images;
    for (auto const& pair : images)
    {
        if (pair.second == "BF")
        {
            _images.push_back(std::pair<Mat_<Vec3b>, QString>(pair.first, "WF"));  // every free interpreted as WF
        }
        else
        {
            _images.push_back(pair);
        }
    }
    return _images;
}
