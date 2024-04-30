#include "../../headers/classification/SupportVectorMachine.h"


SupportVectorMachine::SupportVectorMachine()
{
	// todo
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

int getFeatureMatrixFromImages(std::vector<Mat_<Vec3b>> originalImages, Mat_<float>& X, bool visualize = false, std::vector<int> visualizeImageIndices = std::vector<int>())
{
	// get grayscale version of each image
	std::vector<Mat_<uchar>> grayscaleImages;
	for (Mat_<Vec3b> originalImage : originalImages)
	{
		Mat_<uchar> grayscaleImage;
		cvtColor(originalImage, grayscaleImage, COLOR_BGR2GRAY);
		grayscaleImages.push_back(grayscaleImage);
	}

	if (visualize)
	{
		auto grayscaleImagesConcat = extractAndConcatCellImages(grayscaleImages, visualizeImageIndices);
		imshow("grayscaleImagesConcat", grayscaleImagesConcat);
	}


	// get a vector with hardcoded keypoints, a "grid", same for each image
	std::vector<KeyPoint> keyPoints;
	const int keyPointStepX = 10;
	const int keyPointStepY = 10;
	const int keyPointSize = 1;  // region around the point - could set larger for central pixels - they are more important?
	for (int i = 0; i < grayscaleImages[0].rows; i += keyPointStepY)
	{
		for (int j = 0; j < grayscaleImages[0].cols; j += keyPointStepX)
		{
			keyPoints.push_back(KeyPoint{ (float)j, (float)i, 1 });
		}
	}

	std::vector<Mat_<Vec3b>> keyPointImages;
	for (int i = 0; i < grayscaleImages.size(); i++)
	{
		Mat_<Vec3b> keyPointImage;
		drawKeypoints(grayscaleImages[i], keyPoints, keyPointImage);
		keyPointImages.push_back(keyPointImage);
	}

	if (visualize)
	{
		auto keyPointImagesConcat = extractAndConcatCellImages(keyPointImages, visualizeImageIndices);
		imshow("keyPointImagesConcat", keyPointImagesConcat);
	}


	// get descriptors from keypoints
	//Ptr<xfeatures2d::BriefDescriptorExtractor> brief = xfeatures2d::BriefDescriptorExtractor::create(); finds no features
	// ORB Feature Detector - https://stackoverflow.com/questions/23448420/orb-is-not-detecting-keypoints-in-opencv-2-4-9
	int nfeatures = 500;
	float scaleFactor = 1.2f;
	int nlevels = 8;
	int edgeThreshold = 15; // Changed default (31);
	int firstLevel = 0;
	int WTA_K = 2;
	ORB::ScoreType scoreType = ORB::HARRIS_SCORE;
	int patchSize = 31;
	int fastThreshold = 20;

	Ptr<ORB> orb = ORB::create(nfeatures, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, scoreType, patchSize, fastThreshold);

	std::vector<Mat_<uchar>> descriptors;  // what type - look like uchar
	for (int i = 0; i < grayscaleImages.size(); i++)
	{
		Mat_<uchar> descriptor;
		orb->compute(grayscaleImages[i], keyPoints, descriptor);
		descriptors.push_back(descriptor);
	}

	if (visualize)
	{
		for (int i = 0; i < 3; i++)
		{
			std::cout << "Descriptors " << i << ": " << std::endl;
			std::cout << descriptors[i] << std::endl << std::endl;
		}
	}

	// sanity check
	const int descriptorRows = descriptors[0].rows;
	const int desctiptorCols = descriptors[0].cols;
	for (int i = 1; i < descriptors.size(); i++)
	{
		if (descriptors[i].rows != descriptorRows || descriptors[i].cols != desctiptorCols)
		{
			SPDLOG_ERROR("Descriptor dimensions not matching. dim {}: {}x{}, dim {}: {}x{}",
				0, descriptorRows, desctiptorCols, i, descriptors[i].rows, descriptors[i].cols);
			return 3;
		}
	}

	// flatten descriptors to use as a feature
	Mat_<float> X_(0, descriptorRows * desctiptorCols);  // ml::SVM requires float
	for (Mat_<uchar> descriptor : descriptors)
	{
		Mat_<float> feature(1, descriptorRows * desctiptorCols);

		for (int i = 0; i < descriptor.rows; i++)
		{
			for (int j = 0; j < descriptor.cols; j++)
			{
				feature(0, i * descriptor.cols + j) = static_cast<float>(descriptor(i, j));
			}
		}
		X_.push_back(feature);
	}
	X = X_;

	if (visualize)
	{
		std::cout << "First 3 rows of X";
		for (int i = 0; i < 3; i++)
		{
			std::cout << X.row(i) << std::endl;
		}
		std::cout << std::endl << std::endl;
	}

	return 0;
}

int SupportVectorMachine::train()
{
	/*
	const bool visualizeOnly = true;  // only do steps on one image from each class, or on whole train set
	const bool visualize = true;  // show images with steps

	// get train images
	std::vector<std::pair<Mat_<Vec3b>, QString>> imagesAndLabels;
	if (FileHandler::readLabelFolderImages(TRAIN_FOLDER_PATH, imagesAndLabels) != 0)
	{
		return 1;
	}

	// split images and labels. they should still be "connected by index". labels can already
	//   be put in their "final form". in the meantime, store an index of an image from each class
	std::vector <Mat_<Vec3b>> originalImages;
	Mat_<int> y(0, 1);  // ml::SVM requires int
	std::vector<int> visualizeImageIndices;
	
	for (int i = 0; i < imagesAndLabels.size(); i++)
	{
		uchar label = imagesAndLabels[i].second;

		originalImages.push_back(imagesAndLabels[i].first);
		y.push_back(Mat_<int>(1, 1, label));
		visualizeImageIndices.push_back(originalImages.size() - 1);  // not i, but the index in originalImages
		
		i++;
		while (i < imagesAndLabels.size() && imagesAndLabels[i].second == label)
		{
			if (!visualizeOnly)
			{
				originalImages.push_back(imagesAndLabels[i].first);
				y.push_back(Mat_<uchar>(1, 1, label));
			}
			i++;
		}
	}

	// sanity check
	if (visualizeImageIndices.size() != CLASS_COUNT)
	{
		SPDLOG_ERROR("Unexpected size of visualizeImageIndices ({})!", visualizeImageIndices.size());
		return 2;
	}

	if (visualize)
	{
		auto originalImagesConcat = extractAndConcatCellImages(originalImages, visualizeImageIndices);
		imshow("originalImagesConcat", originalImagesConcat);
	}

	Mat_<float> X;
	if (getFeatureMatrixFromImages(originalImages, X, visualize, visualizeImageIndices) != 0)
	{
		SPDLOG_ERROR("Error getting features");
		return 3;
	}

	// https://docs.opencv.org/4.x/d1/d73/tutorial_introduction_to_svm.html
	svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	
	svm->train(X, ml::ROW_SAMPLE, y);

	if (visualize)
	{
		waitKey();
	}
	*/
	return 0;
}


int SupportVectorMachine::test()
{
	/*
	const bool visualizeOnly = true;  // only do steps on one image from each class, or on whole train set
	const bool visualize = true;  // show images with steps

	// check if valid call
	if (!svm)
	{
		SPDLOG_ERROR("No SVM found, call train first");
		return 1;
	}

	// get test images
	std::vector<std::pair<Mat_<Vec3b>, uchar>> imagesAndLabels;
	if (FileHandler::readLabelFolderImages(TRAIN_FOLDER_PATH, imagesAndLabels) != 0)
	{
		return 2;
	}

	// split images and labels
	std::vector <Mat_<Vec3b>> originalImages;
	Mat_<int> y(0, 1);

	for (int i = 0; i < imagesAndLabels.size(); i++)
	{
		uchar label = imagesAndLabels[i].second;

		originalImages.push_back(imagesAndLabels[i].first);
		y.push_back(Mat_<int>(1, 1, label));
	}

	Mat_<float> X;
	if (getFeatureMatrixFromImages(originalImages, X) != 0)
	{
		SPDLOG_ERROR("Error getting features");
		return 3;
	}

	Ptr<ml::TrainData> testData = ml::TrainData::create(X, ml::ROW_SAMPLE, y);
	Mat_<int> output;  // predicted labels
	float precision = svm->calcError(testData, false, output);
	
	if (visualize)
	{
		std::cout << "Image number - actual class - predicted class" << std::endl;
		for (int i = 0; i < y.rows; i++)
		{
			std::cout << i << " - " << y(i, 0) << " - " << output(i, 0) << "\t";
			if ((i + 1) % 10 == 0)
			{
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
		std::cout << "Precision: " << precision << std::endl;
	}
	*/
	return 0;
}


int SupportVectorMachine::classifyBoard(QVector<QString>& encodings)
{
	/*
	// check if valid call
	if (!svm)
	{
		SPDLOG_ERROR("No SVM found, call train first");
		return 1;
	}

	// read cell images
	std::array<std::array<Mat_<Vec3b>, 8>, 8> boardImages;
	if (FileHandler::readBoardImages(boardImages) != 0)
	{
		return 2;
	}

	// need a flattened version to pass to getFeatureMatrixFromImages
	std::vector<Mat_<Vec3b>> originalImages;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			originalImages.push_back(boardImages[i][j]);
		}
	}

	Mat_<float> X;
	if (getFeatureMatrixFromImages(originalImages, X, false) != 0)
	{
		SPDLOG_ERROR("Error getting features");
		return 3;
	}

	Mat_<int> results;  // predicted labels
	svm->predict(X, results);

	// compose encoding by successive classifications
	encodings.resize(64);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			encodings[i * 8 + j] = QChar::fromLatin1(results(i * 8 + j, 0));  // will change most probably TODO
		}
	}
	*/
	return 0;
}



int SupportVectorMachine::save()
{
	// todo
	return 404;
}


int SupportVectorMachine::load()
{
	// todo
	return 404;
}