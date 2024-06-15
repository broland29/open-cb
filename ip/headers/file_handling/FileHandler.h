#pragma once

#include "../com/headers/Paths.h"
#include "../CommonIP.h"
#include "../../../Common.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <random>

#define DEFAULT_TRAIN_SPLIT			0.6
#define DEFAULT_VALIDATION_SPLIT	0.2
#define DEFAULT_TEST_SPLIT			0.2

class FileHandler
{
public:
	// https://stackoverflow.com/questions/7571278/how-do-i-define-string-constants-in-c
	static const std::string EXTENSION;

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
		std::vector<std::pair<Mat_<Vec3b>, QString>>& imagesAndLabels,	// <image, label> pairs
		std::map<std::string, int>& labelsAndCounts  // additional information about how many images are grouped by labels
	);

	// get all cell images of the board saved beforehand, using boardImageName
	static int readBoardImages(
		std::array<std::array<Mat_<Vec3b>, 8>, 8>& boardImages
	);


	// ---------- other file handling utilities ---------- //

	// wrapper for imwrite to handle errors and use one naming convention over the whole project
	static int saveImage(
		Mat image,						// the image itself
		std::string folder,				// the folder to put the image into
		std::string& path,				// full path where image will be saved (output)
		bool avoidOverwrite,			// add current date to image name, to avoid duplicates. set false for board images
		std::string name = "image"		// actually part of image name, can be used for debug/ identification purposes
	);

	// convention for name parameter of saveImage for BOARD_FOLDER_PATH images
	static std::string boardImageName(int row, int col);

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