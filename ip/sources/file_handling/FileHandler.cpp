#include "../../headers/file_handling/FileHandler.h"

namespace fs = std::experimental::filesystem;


int FileHandler::shuffleAndSplit(double trainSplit, double validationSplit, double testSplit)
{
	// don't need to open images to move them! instead std::vector<std::pair<Mat_<Vec3b>, uchar>>,
	//   where the Mat_ was the image, we can have std::vector<std::string, uchar>>, where string
	//   is the old path. when renamed, the subfolder and image name shall remain the same.

	// get paths and label folders of all images
	std::vector<std::pair<std::string, std::string>> temporaryPALFOld, trainPALFOld, validationPALFOld, testPALFOld;
	if (readLabelFolderPaths(TEMPORARY_FOLDER_PATH, temporaryPALFOld) +
		readLabelFolderPaths(TRAIN_FOLDER_PATH, trainPALFOld) +
		readLabelFolderPaths(VALIDATION_FOLDER_PATH, validationPALFOld) +
		readLabelFolderPaths(TEST_FOLDER_PATH, testPALFOld) != 0)
	{
		return 1;
	}

	// unite all paths
	std::vector<std::pair<std::string, std::string>> PALFOld{};
	PALFOld.insert(PALFOld.end(), temporaryPALFOld.begin(), temporaryPALFOld.end());
	PALFOld.insert(PALFOld.end(), trainPALFOld.begin(), trainPALFOld.end());
	PALFOld.insert(PALFOld.end(), validationPALFOld.begin(), validationPALFOld.end());
	PALFOld.insert(PALFOld.end(), testPALFOld.begin(), testPALFOld.end());
	SPDLOG_TRACE("{} image paths loaded", PALFOld.size());

	// we must not delete the old images, since we shuffle by moving (renaming)

	// shuffle
	// https://cplusplus.com/reference/algorithm/shuffle/
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(PALFOld.begin(), PALFOld.end(), std::default_random_engine(seed));

	// split into three (enough to have indices)
	const int splitOne = PALFOld.size() * trainSplit;
	const int splitTwo = splitOne + PALFOld.size() * validationSplit;
	SPDLOG_TRACE("New split: {}-{}, {}-{}, {}-{}",
		0, splitOne - 1,
		splitOne, splitTwo - 1,
		splitTwo, PALFOld.size()
	);

	// move files by renaming
	for (int i = 0; i < PALFOld.size(); i++)
	{
		std::string labelFolder = PALFOld[i].second; // remains the same

		std::string oldPath = PALFOld[i].first;
		std::string newPath;

		if (i < splitOne)
		{
			newPath = TRAIN_FOLDER_PATH + std::string("\\") + labelFolder;
		}
		else if (i < splitTwo)
		{
			newPath = VALIDATION_FOLDER_PATH + std::string("\\") + labelFolder;
		}
		else
		{
			newPath = TEST_FOLDER_PATH + std::string("\\") + labelFolder;
		}

		try {
			fs::rename(oldPath, newPath);
		}
		catch (fs::filesystem_error& e) {
			SPDLOG_ERROR("Exception when moving file from {} to {}: {}", oldPath, newPath, e.what());
			return 2;
		}
	}

	return 0;
}


int FileHandler::clearAllImages()
{
	if (clearLabeledFolder(TEMPORARY_FOLDER_PATH) +
		clearLabeledFolder(TRAIN_FOLDER_PATH) +
		clearLabeledFolder(VALIDATION_FOLDER_PATH) +
		clearLabeledFolder(TEST_FOLDER_PATH) +
		clearLabeledFolder(BOARD_FOLDER_PATH) != 0)
	{
		return 1;
	}
	
	return 0;
}


int FileHandler::readLabelFolderImages(std::string labelFolderBasePath, std::vector<std::pair<Mat_<Vec3b>, uchar>> imagesAndLabels)
{
	for (std::string labelFolder : labelFolders)
	{
		uchar label;
		EncodingMapperIP::map(labelFolder, label);

		std::string labelFolderPath = labelFolderBasePath + std::string("\\") + labelFolder;
		for (const auto& dirEntry : fs::directory_iterator(labelFolderPath))
		{
			std::string imagePath = dirEntry.path().string();
			Mat_<Vec3b> img = imread(imagePath, IMREAD_COLOR);
			if (img.empty())
			{
				SPDLOG_ERROR("Image at {} not found", imagePath);
				return 1;
			}

			imagesAndLabels.push_back(std::pair<Mat_<Vec3b>, uchar>(img, label));
		}
	}

	SPDLOG_TRACE("{} images loaded", imagesAndLabels.size());

	return 0;
}


int FileHandler::readBoardImages(std::array<std::array<Mat_<Vec3b>, 8>, 8> boardImages)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			std::string imagePath = BOARD_FOLDER_PATH + std::string("\\") + cellImageName(i, j);

			Mat_<Vec3b> img = imread(imagePath, IMREAD_COLOR);
			if (img.empty())
			{
				SPDLOG_ERROR("Image at {} not found", imagePath);
				return 1;
			}

			boardImages[i][j] = img;
		}
	}

	return 0;
}


int FileHandler::readLabelFolderPaths(std::string labelFolderBasePath, std::vector<std::pair<std::string, std::string>> pathsAndLabelFolders )
{
	for (std::string labelFolder : labelFolders)
	{
		std::string labelFolderPath = labelFolderBasePath + std::string("\\") + labelFolder;
		for (const auto& dirEntry : fs::directory_iterator(labelFolderPath))
		{
			pathsAndLabelFolders.push_back(std::pair<std::string, std::string>(dirEntry.path().string(), labelFolder));
		}
	}

	return 0;
}


int FileHandler::clearLabeledFolder(std::string labelFolderBasePath)
{
	// delete whole folder
	if (fs::remove_all(labelFolderBasePath) == -1)  // https://en.cppreference.com/w/cpp/filesystem/remove
	{
		SPDLOG_ERROR("Error when removing folder {}", labelFolderBasePath);
		return 1;
	}

	// re-create train folder
	if (fs::create_directory(labelFolderBasePath) == false)  // https://en.cppreference.com/w/cpp/filesystem/create_directory
	{
		SPDLOG_ERROR("Error when creating folder {}", labelFolderBasePath);
		return 2;
	}

	// re-create subdirectories
	for (std::string labelFolder : labelFolders)
	{
		std::string labelFolderPath = labelFolderBasePath + std::string("\\") + labelFolder;
		if (fs::create_directory(labelFolderPath) == false)  // https://en.cppreference.com/w/cpp/filesystem/create_directory
		{
			SPDLOG_ERROR("Error when creating folder {}", labelFolderPath);
			return 3;
		}
	}

	return 0;
}


