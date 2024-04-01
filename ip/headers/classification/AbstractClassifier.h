#pragma once

#include <string>


class AbstractClassifier
{
public:
	// later: in constructor one could pass hyperparameters as arguments

	// train the classifier. images in TRAIN_FOLDER_PATH and VALIDATION_FOLDER_PATH
	virtual int train() = 0;

	// test the classifier. images in TEST_FOLDER_PATH
	virtual int test() = 0;

	// classify a board. 64 images, in BOARD_FOLDER_PATH. return "" for failure, encodings for success
	virtual std::string classifyBoard() = 0;

	// save classifier to disk
	virtual int save(std::string folderPath) = 0;

	// load classifier from disk
	virtual int load(std::string folderPath) = 0;
};