#include "../../headers/classification/SupportVectorMachine.h"


SupportVectorMachine::SupportVectorMachine(SupportVectorMachineParameters supportVectorMachineParameters) : CxxClassifier(supportVectorMachineParameters.uniteFrees)
{
}


int SupportVectorMachine::train()
{
	SPDLOG_TRACE("Starting training");

	std::vector<std::pair<Mat_<Vec3b>, std::string>> trainImages;
	std::map<std::string, int> trainLabelsAndCounts;
	if (FileHandler::readLabelFolderImages(Paths::TRAIN_FOLDER, trainImages, trainLabelsAndCounts) != 0)
	{
		return 1;
	}

	std::vector<std::pair<Mat_<Vec3b>, std::string>> validationImages;
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

	// get a vector with hardcoded key points, a "grid", same for each image
	keyPoints.clear();
	const int keyPointStepX = 10;
	const int keyPointStepY = 10;
	const int keyPointSize = 1;  // region around the point (idea: could set larger for central pixels since they are more important?)
	for (int i = 0; i < trainImages[0].first.rows; i += keyPointStepY)
	{
		for (int j = 0; j < trainImages[0].first.cols; j += keyPointStepX)
		{
			keyPoints.push_back(KeyPoint{ static_cast<float>(j), static_cast<float>(i), 1 });
		}
	}

	getFeaturesAndLabels(trainImages, X, y);

	if (DEBUG)
	{
		SPDLOG_TRACE("Built {} features", X.rows);
		logExampleFeatures();
	}

	// https://docs.opencv.org/4.x/d1/d73/tutorial_introduction_to_svm.html
	svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

	svm->train(X, ml::ROW_SAMPLE, y);
	trained = true;
	SPDLOG_TRACE("Finished training");

	return 0;
}


int SupportVectorMachine::test()
{
	if (!trained)
	{
		SPDLOG_ERROR("Should train first");
		return 1;
	}

	std::vector<std::pair<Mat_<Vec3b>, std::string>> testImages;
	std::map<std::string, int> labelsAndCounts;
	if (FileHandler::readLabelFolderImages(Paths::TEST_FOLDER, testImages, labelsAndCounts) != 0)
	{
		return 2;
	}

	if (DEBUG)
	{
		logDistribution(labelsAndCounts, "test");
	}

	// confustion matrix: on x axis we have predicted class, on y we have actual class
	std::vector<std::vector<int>> confusionMatrix;

	confusionMatrix.resize(classCount);
	for (int i = 0; i < classCount; i++)
	{
		confusionMatrix[i].resize(classCount, 0);
	}

	Mat_<float> features;		// feature matrix
	Mat_<int> actualClasses;	// class labels
	getFeaturesAndLabels(testImages, features, actualClasses);

	Mat_<int> predictedClasses;


	const int testSize = testImages.size();
	for (int i = 0; i < testSize; i++)
	{
		int predictedClass = svm->predict(features.row(i));

		confusionMatrix[actualClasses(i, 0)][predictedClass]++;  // using class directly to access slot!

		// just to keep the console interactive
		if (i % 100 == 0)
		{
			SPDLOG_TRACE("{}/{}", i, testSize);
		}
	}

	// even if BF and WF are treated separately, in the long run, they end up as the same "category". so it
	// is only fair to unite the rows and columns of the confusion matrix with BF and WF before calculating metrics
	if (!uniteFrees)
	{
		std::vector<std::vector<int>> confusionMatrixNew(13, std::vector<int>(13));

		// unite WF and BF on first row/col "as FR"
		const int indexWF = externalToInternal("WF");
		const int indexBF = externalToInternal("BF");
		confusionMatrixNew[0][0] = confusionMatrix[indexWF][indexWF] + confusionMatrix[indexBF][indexBF];
		for (int i = 0; i < 13; i++)
		{
			if (i == indexWF || i == indexBF)
			{
				continue;
			}
			confusionMatrixNew[0][i] = confusionMatrix[indexWF][i] + confusionMatrix[indexBF][i];
			confusionMatrixNew[i][0] = confusionMatrix[i][indexWF] + confusionMatrix[i][indexBF];
		}

		// the rest of the encodings from AbstractClassifier::labels is identical with FileHandler::labelFolderNames and can be "copied"
		for (int i = 1; i < 13; i++)
		{
			for (int j = 1; j < 13; j++)
			{
				confusionMatrixNew[i][j] = confusionMatrix[externalToInternal(labels[i])][externalToInternal(labels[j])];
			}
		}
		confusionMatrix = confusionMatrixNew;
	}
	calculateAndLogMetrics(confusionMatrix);

	return 0;
}


int SupportVectorMachine::classifyBoard(QVector<QString>& encodings)
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
			encodings[i * 8 + j] = internalToExternal(svm->predict(getFeatureFromImage(boardImages[i][j])));
		}
	}

	return 0;
}


int SupportVectorMachine::save()
{
	if (!trained)
	{
		SPDLOG_ERROR("Should train first");
		return 1;
	}

	std::string path = Paths::SVM_FOLDER + std::string("\\svm.txt");
	SPDLOG_TRACE("Saving to {}", path);
	svm->save(path);

	return 0;
}


int SupportVectorMachine::load()
{
	std::string path = Paths::SVM_FOLDER + std::string("\\svm.txt");
	SPDLOG_TRACE("Loading from {}", path);
	svm = Algorithm::load<ml::SVM>(path);
	trained = true;  // since saving only works if trained

	return 0;
}


Mat_<float> SupportVectorMachine::getFeatureFromImage(Mat_<Vec3b> image)
{
	// get grayscale version of image
	Mat_<uchar> grayscaleImage;
	cvtColor(image, grayscaleImage, COLOR_BGR2GRAY);

	// get descriptor
	Ptr<ORB> orb = ORB::create(N_FEATURES, SCALE_FACTOR, N_LEVELS, EDGE_THRESHOLD, FIRST_LEVEL, WTA_K, SCORE_TYPE, PATCH_SIZE, FAST_THRESHOLD);
	Mat_<uchar> descriptor;  // not sure about type
	orb->compute(grayscaleImage, keyPoints, descriptor);

	// flatten descriptor to use as a feature
	Mat_<float> feature(1, descriptor.rows * descriptor.cols);
	for (int i = 0; i < descriptor.rows; i++)
	{
		for (int j = 0; j < descriptor.cols; j++)
		{
			feature(0, i * descriptor.cols + j) = static_cast<float>(descriptor(i, j));
		}
	}

	return feature;
}


void SupportVectorMachine::getFeaturesAndLabels(std::vector<std::pair<Mat_<Vec3b>, std::string>> images, Mat_<float>& X, Mat_<int>& y)
{
	// length of feature is fixed for each image
	int d = getFeatureFromImage(images[0].first).cols;

	// unknown number of images -> unknown number of rows -> start with 0 and push_back
	Mat_<float> _X(0, d);  // feature matrix
	Mat_<int> _y(0, 1);   // class labels

	for (auto const& pair : images)
	{
		Mat_<Vec3b> image = pair.first;
		int label = externalToInternal(pair.second);  // possible unification of frees done here

		Mat_<float> feature = getFeatureFromImage(image);
		_X.push_back(feature);

		_y.push_back(Mat_<int>(1, 1, label));
	}

	X = _X.clone();
	y = _y.clone();
}


void SupportVectorMachine::logExampleFeatures()
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