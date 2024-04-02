#include "../../headers/file_handling/FileHandler.h"

namespace fs = std::experimental::filesystem;


int FileHandler::shuffleAndSplit(double trainSplit, double validationSplit, double testSplit)
{
	// don't need to open images to move them! instead std::vector<std::pair<Mat_<Vec3b>, uchar>>,
	//   where the Mat_ was the image, we can have std::vector<std::string, uchar>>, where string
	//   is the old path. when renamed, the subfolder and image name shall remain the same.

	// get paths and label folders of all images
	std::vector<PathInfo> temporaryPathInfosOld, trainPathInfosOld, validationPathInfosOld, testPathInfosOld;
	if (readLabelFolderPathInfos(TEMPORARY_FOLDER_PATH, temporaryPathInfosOld) +
		readLabelFolderPathInfos(TRAIN_FOLDER_PATH, trainPathInfosOld) +
		readLabelFolderPathInfos(VALIDATION_FOLDER_PATH, validationPathInfosOld) +
		readLabelFolderPathInfos(TEST_FOLDER_PATH, testPathInfosOld) != 0)
	{
		return 1;
	}

	// unite all paths
	std::vector<PathInfo> pathInfosOld{};
	pathInfosOld.insert(pathInfosOld.end(), temporaryPathInfosOld.begin(), temporaryPathInfosOld.end());
	pathInfosOld.insert(pathInfosOld.end(), trainPathInfosOld.begin(), trainPathInfosOld.end());
	pathInfosOld.insert(pathInfosOld.end(), validationPathInfosOld.begin(), validationPathInfosOld.end());
	pathInfosOld.insert(pathInfosOld.end(), testPathInfosOld.begin(), testPathInfosOld.end());
	SPDLOG_TRACE("{} image paths loaded", pathInfosOld.size());

	// we must not delete the old images, since we shuffle by moving (renaming)

	// shuffle
	// https://cplusplus.com/reference/algorithm/shuffle/
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(pathInfosOld.begin(), pathInfosOld.end(), std::default_random_engine(seed));

	// split into three (enough to have indices)
	const int splitOne = pathInfosOld.size() * trainSplit;
	const int splitTwo = splitOne + pathInfosOld.size() * validationSplit;
	SPDLOG_TRACE("New split:");
	SPDLOG_TRACE("Train:		{} images ({}-{})", splitOne, 0, splitOne-1);
	SPDLOG_TRACE("Validation:	{} images ({}-{})", splitTwo - splitOne, splitOne, splitTwo - 1);
	SPDLOG_TRACE("Test:			{} images ({}-{})", pathInfosOld.size() - splitTwo, splitTwo, pathInfosOld.size() - 1);


	// move files by renaming
	for (int i = 0; i < pathInfosOld.size(); i++)
	{
		std::string labelFolderBasePathNew;
		if (i < splitOne)
		{
			labelFolderBasePathNew = TRAIN_FOLDER_PATH;
		}
		else if (i < splitTwo)
		{
			labelFolderBasePathNew = VALIDATION_FOLDER_PATH;
		}
		else
		{
			labelFolderBasePathNew = TEST_FOLDER_PATH;
		}

		std::string oldPath = pathInfosOld[i].wholePath;
		std::string newPath = 
			labelFolderBasePathNew + std::string("\\") + 
			pathInfosOld[i].labelFolder + std::string("\\") +
			pathInfosOld[i].imageName;

		try
		{
			fs::rename(oldPath, newPath);
		}
		catch (fs::filesystem_error& e)
		{
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
		clearSimpleFolder(BOARD_FOLDER_PATH) != 0)
	{
		return 1;
	}
	
	return 0;
}


int FileHandler::readLabelFolderImages(std::string labelFolderBasePath, std::vector<std::pair<Mat_<Vec3b>, uchar>>& imagesAndLabels)
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


int FileHandler::readBoardImages(std::array<std::array<Mat_<Vec3b>, 8>, 8>& boardImages)
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


int FileHandler::readLabelFolderPathInfos(std::string labelFolderBasePath, std::vector<PathInfo>& pathInfos)
{
	if (!fs::exists(labelFolderBasePath))
	{
		if (clearLabeledFolder(labelFolderBasePath) != 0)
		{
			return 1;
		}

		SPDLOG_WARN("Folder {} did not exist, created", labelFolderBasePath);
		return 0;  // not much to iterate if base folder was not even there
	}

	for (std::string labelFolder : labelFolders)
	{
		std::string labelFolderPath = labelFolderBasePath + std::string("\\") + labelFolder;
		for (const auto& dirEntry : fs::directory_iterator(labelFolderPath))
		{
			pathInfos.push_back(PathInfo {
				dirEntry.path().string(),
				labelFolder,
				dirEntry.path().filename().string() }
			);
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


int FileHandler::clearSimpleFolder(std::string simpleFolderBasePath)
{
	// delete folder
	if (fs::remove_all(simpleFolderBasePath) == -1)
	{
		SPDLOG_ERROR("Error when removing folder {}", simpleFolderBasePath);
		return 1;
	}

	// re-create folder
	if (fs::create_directory(simpleFolderBasePath) == false)
	{
		SPDLOG_ERROR("Error when creating folder {}", simpleFolderBasePath);
		return 2;
	}

	return 0;
}


