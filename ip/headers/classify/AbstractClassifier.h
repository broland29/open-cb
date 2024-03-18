#pragma once

#include "../Util.h"

class AbstractClassifier
{
public:
	// get feature matrix, class labels and train the classifier
	virtual void train(std::vector<std::pair<Mat_<Vec3b>, uchar>> trainImages) = 0;

	// classify a single image
	virtual uchar classify(Mat_<Vec3b> image) = 0;
};