#include "../../headers/classification/KNearestNeighbors.h"


KNearestNeighbors::KNearestNeighbors(KNearestNeighborsParameters kNearestNeighborsParameters)
{
    k = kNearestNeighborsParameters.k;
    numberOfBins = kNearestNeighborsParameters.numberOfBins;
    uniteFrees = kNearestNeighborsParameters.uniteFrees;
    trained = false;
}


int KNearestNeighbors::train()
{
    SPDLOG_TRACE("Starting training");
    
    std::vector<std::pair<Mat_<Vec3b>, QString>> trainImages;
    std::map<std::string, int> trainLabelsAndCounts;
    if (FileHandler::readLabelFolderImages(Paths::TRAIN_FOLDER, trainImages, trainLabelsAndCounts) != 0)
    {
        return 1;
    }

    std::vector<std::pair<Mat_<Vec3b>, QString>> validationImages;
    std::map<std::string, int> validationLabelsAndCounts;
    if (FileHandler::readLabelFolderImages(Paths::VALIDATION_FOLDER, validationImages, validationLabelsAndCounts) != 0)
    {
        return 2;
    }

    // since validation not used in other ways, add to train images
    trainImages.insert(trainImages.end(), validationImages.begin(), validationImages.end());

    if (DEBUG)
    {
        for (auto const& validationLabelAndCount : validationLabelsAndCounts)
        {
            trainLabelsAndCounts[validationLabelAndCount.first] += validationLabelAndCount.second;
        }
        logDistribution(trainLabelsAndCounts, "train");
    }

    const int m = numberOfBins;     // number of bins in histogram(s)
    const int d = 3 * m;            // number of features, width of X

    // unknown number of images -> unknown number of rows -> start with 0 and push_back
    Mat_<int> X(0, d);  // feature matrix
    Mat_<int> y(0, 1);   // class labels

    for (auto const& pair : trainImages)
    {
        Mat_<Vec3b> image = pair.first;
        int label = externalToInternal(pair.second);  // possible unification of frees done here

        Mat_<int> feature = getFeatureHistogram(image);
        X.push_back(feature);

        Mat_<int> label_(1, 1);
        label_(0, 0) = label;
        y.push_back(label);
    }

    // store X and y, since will need it later
    this->X = X.clone();
    this->y = y.clone();

    if (DEBUG)
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

    std::vector<std::pair<Mat_<Vec3b>, QString>> testImages;
    std::map<std::string, int> labelsAndCounts;
    if (FileHandler::readLabelFolderImages(Paths::TEST_FOLDER, testImages, labelsAndCounts) != 0)
    {
        return 2;
    }

    if (DEBUG)
    {
        logDistribution(labelsAndCounts, "test");
    }

    int classCount = (uniteFrees) ? 13 : 14;

    // confustion matrix: on x axis we have predicted class, on y we have actual class
    std::vector<std::vector<int>> confusionMatrix(classCount);
       for (int i = 0; i < classCount; i++)
    {
        confusionMatrix[i].resize(classCount, 0);
    }

    const int testSize = testImages.size();
    for (int i = 0; i < testSize; i++)
    {
        Mat_<Vec3b> img = testImages[i].first;
        int actualClass = externalToInternal(testImages[i].second);	 // possible unification of frees done here
        int predictedClass = classify(img);

        confusionMatrix[actualClass][predictedClass]++;  // using class directly to access slot!

        // just to keep the console interactive
        if (i % 100 == 0)
        {
            SPDLOG_TRACE("{}/{}", i, testSize);
        }
    }

    std::vector<std::string> encodings(classCount);
    for (int i = 0; i < classCount; i++)
    {
        encodings[i] = internalToExternal(i).toStdString();
    }
    calculateAndLogMetrics(confusionMatrix, encodings);

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

    std::string path = Paths::KNN_FOLDER + std::string("\\knn.txt");
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
    std::string path = Paths::KNN_FOLDER + std::string("\\knn.txt");
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


void KNearestNeighbors::getHistogram(Mat_<Vec3b> img, int colorIndex, Mat_<int> hist)
{
    for (int i = 0; i < numberOfBins; i++)
    {
        hist(0, i) = 0;
    }

    int binSize = 256 / numberOfBins;
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            hist(0, img.at<Vec3b>(i, j)[colorIndex] / binSize)++;
        }
    }
}


Mat_<int> KNearestNeighbors::getFeatureHistogram(Mat_<Vec3b> img)
{
    Mat_<int> feature(1, numberOfBins * 3);

    for (int i = 0; i < 3; i++)  // colors
    {
        Mat_<int> histogram(1, numberOfBins);
        getHistogram(img, i, histogram);
        for (int j = 0; j < numberOfBins; j++)
        {
            feature(0, i * numberOfBins + j) = histogram(0, j);
        }
    }

    return feature;
}


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
