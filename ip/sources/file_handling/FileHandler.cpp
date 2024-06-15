#include "../../headers/file_handling/FileHandler.h"

namespace fs = std::experimental::filesystem;


const std::string FileHandler::EXTENSION = ".jpeg";


int FileHandler::shuffleAndSplit(double trainSplit, double validationSplit, double testSplit)
{
	// don't need to open images to move them! instead std::vector<std::pair<Mat_<Vec3b>, uchar>>,
	//   where the Mat_ was the image, we can have std::vector<std::string, uchar>>, where string
	//   is the old path. when renamed, the subfolder and image name shall remain the same.

	// get paths and label folders of all images
	std::vector<PathInfo> temporaryPathInfosOld, trainPathInfosOld, validationPathInfosOld, testPathInfosOld;
	if (readLabelFolderPathInfos(Paths::TEMPORARY_FOLDER, temporaryPathInfosOld) +
		readLabelFolderPathInfos(Paths::TRAIN_FOLDER, trainPathInfosOld) +
		readLabelFolderPathInfos(Paths::VALIDATION_FOLDER, validationPathInfosOld) +
		readLabelFolderPathInfos(Paths::TEST_FOLDER, testPathInfosOld) != 0)
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
			labelFolderBasePathNew = Paths::TRAIN_FOLDER;
		}
		else if (i < splitTwo)
		{
			labelFolderBasePathNew = Paths::VALIDATION_FOLDER;
		}
		else
		{
			labelFolderBasePathNew = Paths::TEST_FOLDER;
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
	if (clearLabeledFolder(Paths::TEMPORARY_FOLDER) +
		clearLabeledFolder(Paths::TRAIN_FOLDER) +
		clearLabeledFolder(Paths::VALIDATION_FOLDER) +
		clearLabeledFolder(Paths::TEST_FOLDER) +
		clearSimpleFolder(Paths::BOARD_FOLDER) + 
		clearSimpleFolder(Paths::GRAB_FOLDER) != 0)
	{
		return 1;
	}
	
	return 0;
}


int FileHandler::readLabelFolderImages(std::string labelFolderBasePath, std::vector<std::pair<Mat_<Vec3b>, QString>>& imagesAndLabels, std::map<std::string, int>& labelsAndCounts)
{
	for (QString encoding : ENCODINGS)
	{
		labelsAndCounts.insert({ encoding.toStdString(), 0 });
	}

	for (QString encoding : ENCODINGS)
	{
		std::string labelFolder = encoding.toStdString();

		std::string labelFolderPath = labelFolderBasePath + std::string("\\") + labelFolder;
		for (const auto& dirEntry : fs::directory_iterator(labelFolderPath))
		{
			labelsAndCounts[labelFolder]++;
			std::string imagePath = dirEntry.path().string();
			Mat_<Vec3b> img = imread(imagePath, IMREAD_COLOR);
			if (img.empty())
			{
				SPDLOG_ERROR("Image at {} not found", imagePath);
				return 1;
			}

			imagesAndLabels.push_back(std::pair<Mat_<Vec3b>, QString>(img, encoding));
		}
	}

	SPDLOG_TRACE("Loaded {} images from {}", imagesAndLabels.size(), labelFolderBasePath);

	return 0;
}


int FileHandler::readBoardImages(std::array<std::array<Mat_<Vec3b>, 8>, 8>& boardImages)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			std::string imagePath = Paths::BOARD_FOLDER + std::string("\\") + boardImageName(i, j) + EXTENSION;

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

	for (QString encoding : ENCODINGS)
	{
		std::string labelFolder = encoding.toStdString();
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
	for (QString encoding : ENCODINGS)
	{
		std::string labelFolder = encoding.toStdString();
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


int FileHandler::saveImage(Mat image, std::string folder, std::string& path, bool avoidDuplicates, std::string name)
{
	std::string now = "";
	if (avoidDuplicates)  // acceptable to assume that no two saveImage operations will happen in the same millisecond
	{
		// https://stackoverflow.com/questions/9089842/c-chrono-system-time-in-milliseconds-time-operations
		now = std::string("_") + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}

	path = folder + std::string("\\") + name + now + ".jpeg";
	SPDLOG_TRACE("Composed path {}", path);

	// imwrite and error handling
	// https://docs.opencv.org/3.4/d4/da8/group__imgcodecs.html#gabbc7ef1aa2edfaa87772f1202d67e0ce
	int result = false;
	try
	{
		result = imwrite(path, image);
	}
	catch (const cv::Exception& ex)
	{
		SPDLOG_ERROR("Conversion for {} failed: exception {}!", path, ex.what());
		return 1;
	}

	if (!result)
	{
		SPDLOG_ERROR("Saving for {} failed!", path);
		return 2;
	}

	return 0;
}


std::string FileHandler::boardImageName(int row, int col)
{
	return "board" + std::to_string(row) + std::to_string(col);
}

