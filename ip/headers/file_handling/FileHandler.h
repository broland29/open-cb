#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <random>

#include "../com/headers/Paths.h"
#include "../CommonIP.h"


class FileHandler
{
public:
	static const std::string EXTENSION;

private:
	// "Standard behavior: Windows file system treats file and directory names as case-insensitive"
	//		- unfortunately, cannot use uppercase-lowercase (like P for white and p for black pawn), we must use WP and BP
	//		- these encodings are used only for the folders; there is no logical difference between WF and BF
	static const std::vector<std::string> labelFolderNames;

	// struct to hold information needed when moving a labeled image. to avoid parsing
	struct PathInfo
	{
		std::string wholePath;			// needed to call rename, as old path
		std::string labelFolderName;	// needed to put in corresponding labelFolder (remains the same)
		std::string imageName;			// needed to keep name (remains the same)
	};

public:
	// gather images from TEMPORARY_FOLDER, TRAIN_FOLDER, VALIDATION_FOLDER and TEST_FOLDER
	// and re-distribute them into TRAIN_FOLDER, VALIDATION_FOLDER and TEST_FOLDER based on parameters
	static int shuffleAndSplit(
		double trainSplit,			// percent of total images to be put in Paths::TRAIN_FOLDER. ex: 0.6
		double validationSplit,		// ex: 0.2
		double testSplit			// ex: 0.2
	);

	// delete all images from TEMPORARY_FOLDER, TRAIN_FOLDER, VALIDATION_FOLDER, TEST_FOLDER, BOARD_FOLDER and GRAB_FOLDER
	static int clearAllImages();

	// reads all images from labelFolderBasePath
	static int readLabelFolderImages(
		std::string labelFolderBasePath,										// TEMPORARY_FOLDER / TRAIN_FOLDER / VALIDATION_FOLDER / TEST_FOLDER
		std::vector<std::pair<Mat_<Vec3b>, std::string>>& imagesAndLabels,		// <image, label> pairs
		std::map<std::string, int>& labelsAndCounts								// additional information about how many images are grouped by labels
	);

	// get all cell images of the board saved beforehand, using boardImageName
	static int readBoardImages(
		std::array<std::array<Mat_<Vec3b>, 8>, 8>& boardImages
	);

	// wrapper for imwrite to handle errors and use one naming convention over the whole project
	static int saveImage(
		Mat image,						// the image itself
		std::string folder,				// the folder to put the image into
		std::string& path,				// full path where image will be saved (output)
		bool avoidOverwrite,			// add current date to image name, to avoid duplicates. set false for board images
		std::string name = "image"		// actually part of image name, can be used for debug/ identification purposes
	);

	// convention for name parameter of saveImage for BOARD_FOLDER images
	static std::string boardImageName(int row, int col);

private:
	// utility for shuffleAndSplit
	static int readLabelFolderPathInfos(
		std::string labelFolderBasePath,	// TEMPORARY_FOLDER / TRAIN_FOLDER / VALIDATION_FOLDER / TEST_FOLDER
		std::vector<PathInfo>& pathInfos	// output
	);

	// utility for clearAllImages
	static int clearLabeledFolder(
		std::string labelFolderBasePath		// TEMPORARY_FOLDER / TRAIN_FOLDER / VALIDATION_FOLDER / TEST_FOLDER
	);

	// utility for clearAllImages
	static int clearSimpleFolder(
		std::string simpleFolderBasePath	// BOARD_FOLDER
	);
};