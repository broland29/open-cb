#include "../../headers/classify/Classify.h"

#define SPLIT_TRAIN true


namespace fs = std::experimental::filesystem;

Classify::Classify()
{
	classifier = new KNearestNeighborsClassifier(5);

	std::string path, path2;
	uchar label;


	if (SPLIT_TRAIN)
	{
		std::vector < std::pair<Mat_<Vec3b>, uchar>> images;
		for (std::string folder : folders)
		{
			path = TRAIN_FOLDER_PATH + std::string("\\") + folder;
			EncodingMapperIP::map(folder, label);

			for (const auto& dirEntry : fs::directory_iterator(path))
			{
				path2 = dirEntry.path().string();
				Mat_<Vec3b> img = imread(path2, IMREAD_COLOR);
				if (img.empty())
				{
					SPDLOG_ERROR("Image at {} not found", path2);
					continue;
				}
				
				images.push_back(std::pair<Mat_<Vec3b>, uchar>(img, label));
			}
		}
		SPDLOG_TRACE("{} images loaded", images.size());

		// https://cplusplus.com/reference/algorithm/shuffle/
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(images.begin(), images.end(), std::default_random_engine(seed));

		// https://stackoverflow.com/questions/9811235/best-way-to-split-a-vector-into-two-smaller-arrays
		std::size_t const trainSize = images.size() * TRAIN_SPLIT;
		trainImages = std::vector<std::pair<Mat_<Vec3b>, uchar>>(images.begin(), images.begin() + trainSize);
		testImages = std::vector<std::pair<Mat_<Vec3b>, uchar>>(images.begin() + trainSize, images.end());
	}
}


int Classify::runTrain()
{
	SPDLOG_TRACE("First five train labels: {} {} {} {} {}",
		trainImages[0].second, trainImages[1].second, trainImages[2].second, trainImages[3].second, trainImages[4].second);
	classifier->train(trainImages);
	return 0;
}

int Classify::runTest()
{
	SPDLOG_TRACE("First five test labels: {} {} {} {} {}",
		testImages[0].second, testImages[1].second, testImages[2].second, testImages[3].second, testImages[4].second);
	
	Mat_<double> confusionMatrix(CLASS_COUNT, CLASS_COUNT, 0.0);  // row: predicted class  col: actual class
	for (auto const& pair : testImages)
	{
		Mat_<Vec3b> img = pair.first;		// vector of images
		uchar actualClass = pair.second;	// class
		uchar predictedClass = classifier->classify(img);

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

	SPDLOG_INFO("Train image count: {}", trainImages.size());
	SPDLOG_INFO("Test image count: {}", testImages.size());
	
	// trickery: spdlog allows string, Mat can be printed
	SPDLOG_INFO("Confusion matrix:");
	std::string str = "\n";
	str << confusionMatrix;
	SPDLOG_INFO(str);

	SPDLOG_INFO("Accuracy: {}", accuracy);
	return 0;
}