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
		std::vector<std::pair<Mat_<Vec3b>, uchar>> imagesAndLabels	// <image, label> pairs
	);

	// get all cell images of the board saved beforehand, using boardImageName
	static int readBoardImages(
		std::array<std::array<Mat_<Vec3b>, 8>, 8> boardImages
	);

	static int readLabelFolderPaths(
		std::string labelFolderBasePath,										// TEMPORARY/TRAIN/VALIDATION/TEST_FOLDER_PATH
		std::vector<std::pair<std::string, std::string>> pathsAndLabelFolders   // <path, label_folder> pairs
	);

	static int clearLabeledFolder(
		std::string labelFolderBasePath  // TEMPORARY/TRAIN/VALIDATION/TEST_FOLDER_PATH
	);
};