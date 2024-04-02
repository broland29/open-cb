#pragma once

#include "../Common.h"
#include "../EncodingMapperIP.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <random>

#define DEFAULT_TRAIN_SPLIT			0.6
#define DEFAULT_VALIDATION_SPLIT	0.2
#define DEFAULT_TEST_SPLIT			0.2

class FileHandler
{
public:
private:
	// all static

	// struct to hold information needed when moving a labeled image. to avoid parsing
	struct PathInfo
	{
		std::string wholePath;		// needed to call rename, as old path
		std::string labelFolder;	// needed to but in corresponding labelFolder (remains the same)
		std::string imageName;		// needed to keep name (remains the same)
	};

public:
	// ---------- functions implementing direct functionalities ---------- //

	// take images from LABEL_FOLDER_PATH, shuffle, and split in TRAIN-, VALIDATION-, TEST_FOLDER_PATH
	static int shuffleAndSplit(
		double trainSplit = DEFAULT_TRAIN_SPLIT,
		double validationSplit = DEFAULT_VALIDATION_SPLIT,
		double testSplit = DEFAULT_TEST_SPLIT
	);

	static int clearAllImages();


	// ---------- functions implementing helper functions for classification ---------- //

	static int readLabelFolderImages(
		std::string labelFolderBasePath,						    // TEMPORARY/TRAIN/VALIDATION/TEST_FOLDER_PATH
		std::vector<std::pair<Mat_<Vec3b>, uchar>>& imagesAndLabels	// <image, label> pairs
	);

	// get all cell images of the board saved beforehand, using boardImageName
	static int readBoardImages(
		std::array<std::array<Mat_<Vec3b>, 8>, 8>& boardImages
	);

private:
	static int clearLabeledFolder(
		std::string labelFolderBasePath  // TEMPORARY/TRAIN/VALIDATION/TEST_FOLDER_PATH
	);

	static int clearSimpleFolder(
		std::string simpleFolderBasePath  // BOARD_FOLDER_PATH
	);

	static int readLabelFolderPathInfos(
		std::string labelFolderBasePath,  // TEMPORARY/TRAIN/VALIDATION/TEST_FOLDER_PATH
		std::vector<PathInfo>& pathInfos
	);
};