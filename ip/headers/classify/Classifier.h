#pragma once

#include "../Util.h"

class Classifier
{
public:
	// get feature matrix, class labels and train the classifier
	virtual void train(std::vector<std::pair<Mat_<Vec3b>, Piece>> trainImages) = 0;

	// classify a single image
	virtual Piece classify(Mat_<Vec3b> image) = 0;

	// classify a set of images and provide analysis of results
	virtual void test(std::vector<Mat_<Vec3b>> testImages) = 0;
};