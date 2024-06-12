#include "../../headers/classification/SupportVectorMachine.h"


SupportVectorMachine::SupportVectorMachine(SupportVectorMachineParameters supportVectorMachineParameters)
{
	uniteFrees = supportVectorMachineParameters.uniteFrees;
	trained = false;
}



// put images in one big image, of size noOfRows * rowSize, noOfCols * colSize.
// if not enough images to fill, padded with zeros. if more, just ignores the rest
template <typename T>
Mat_<T> concatImages(std::vector<Mat_<T>> images, int noOfRows, int noOfCols, int rowSize, int colSize)
{
	Mat_<T> result = Mat_<T>::zeros(noOfRows * rowSize, noOfCols * colSize);
	for (int i = 0; i < noOfRows; i++)
	{
		for (int j = 0; j < noOfCols; j++)
		{
			int flat = i * noOfCols + j;  // tricky but tested
			if (flat >= images.size())
			{
				goto _skip;
			}
			Mat_<T> image = images[flat].clone();

			for (int u = 0; u < image.rows; u++)
			{
				for (int v = 0; v < image.cols; v++)
				{
					result(i * rowSize + u, j * colSize + v) = image(u, v);
				}
			}
		}
	}
_skip:
	return result;
}

template <typename T>
Mat_<T> extractAndConcatCellImages(std::vector<Mat_<T>> images, std::vector<int> indices)
{
	std::vector<Mat_<T>> extractedImages;
	for (int index : indices)
	{
		extractedImages.push_back(images[index]);
	}
	return concatImages(extractedImages, 3, 5, extractedImages[0].rows, extractedImages[0].cols);
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


void SupportVectorMachine::getFeaturesAndLabels(std::vector<std::pair<Mat_<Vec3b>, QString>> images, Mat_<float>& X, Mat_<int>& y)
{
	// length of feature is fixed for each image
	int d = getFeatureFromImage(images[0].first).cols;

	// unknown number of images -> unknown number of rows -> start with 0 and push_back
	Mat_<int> _X(0, d);  // feature matrix
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





int SupportVectorMachine::train()
{
	std::vector<std::pair<Mat_<Vec3b>, QString>> trainImages;
	if (FileHandler::readLabelFolderImages(TRAIN_FOLDER_PATH, trainImages) != 0)
	{
		return 1;
	}

	// since validation not used in other ways, for SVM I use those images as train images as well
	std::vector<std::pair<Mat_<Vec3b>, QString>> _validationImages;
	if (FileHandler::readLabelFolderImages(VALIDATION_FOLDER_PATH, _validationImages) != 0)
	{
		return 2;
	}
	trainImages.insert(trainImages.end(), _validationImages.begin(), _validationImages.end());

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

	if (debug)
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

	std::vector<std::pair<Mat_<Vec3b>, QString>> testImages;
	if (FileHandler::readLabelFolderImages(TEST_FOLDER_PATH, testImages) != 0)
	{
		return 2;
	}

	int classCount = ENCODINGS.size();

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

	calculateAndLogMetrics(confusionMatrix, testSize);
	
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

	std::string path = getPath();
	SPDLOG_TRACE("Saving to {}", path);
	svm->save(path);
	
	return 0;
}


int SupportVectorMachine::load()
{
	std::string path = getPath();
	SPDLOG_TRACE("Loading from {}", path);
	svm = Algorithm::load<ml::SVM>(path);
	trained = true;  // since saving only works if trained

	return 0;
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