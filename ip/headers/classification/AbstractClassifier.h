#pragma once

#include <QVector>
#include <QString>
#include <sstream>
#include <iomanip>
#include <array>

#include "../com/headers/Paths.h"
#include "../com/headers/Parameters.h"


class AbstractClassifier
{
public:

private:

protected:
	// the final labels (classes) we are interested in, independently of FileHandler::labelFolderNames and the state of uniteFrees
	static const std::array<std::string, 13> labels;

public:
	// train the classifier. images in TRAIN_FOLDER_PATH and VALIDATION_FOLDER_PATH
	virtual int train() = 0;

	// test the classifier. images in TEST_FOLDER_PATH
	virtual int test() = 0;

	// classify a board. 64 images, in BOARD_FOLDER_PATH
	virtual int classifyBoard(QVector<QString>& encodings) = 0;

	// save classifier to disk
	virtual int save() = 0;

	// load classifier from disk
	virtual int load() = 0;

private:

protected:
	// the confusion matrix should obey the following convention:
	//		- on rows we have actual classes, on columns we have predicted classes
	//		- on index i, we have label confusionMatrixLabels[i]
	void calculateAndLogMetrics(
		std::vector<std::vector<int>> confusionMatrix  // confusionMatrix[actualClass][predictedClass]
	);
};