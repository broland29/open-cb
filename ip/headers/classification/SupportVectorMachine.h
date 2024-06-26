#pragma once

#include <opencv2/xfeatures2d.hpp>
#include <opencv2/ml.hpp>

#include "CxxClassifier.h"


class SupportVectorMachine : public CxxClassifier
{
public:

private:
	std::vector<KeyPoint> keyPoints;
	Ptr<ORB> orb;
	Ptr<ml::SVM> svm;

	Mat_<float> X;      // feature matrix	flattened ORB descriptors matrix							float for svm to accept it
	Mat_<int> y;        // class labels		0, 1, ..., see internalToExternal, externalToInternal		int for svm to accept it

	// https://docs.opencv.org/3.4/db/d95/classcv_1_1ORB.html#adc371099dc902a9674bd98936e79739c
	const int N_FEATURES = 500;
	const float SCALE_FACTOR = 1.2f;
	const int N_LEVELS = 8;
	const int EDGE_THRESHOLD = 15;  // default 31
	const int FIRST_LEVEL = 0;
	const int WTA_K = 2;
	const ORB::ScoreType SCORE_TYPE = ORB::HARRIS_SCORE;
	const int PATCH_SIZE = 31;
	const int FAST_THRESHOLD = 20;

public:
	SupportVectorMachine(SupportVectorMachineParameters supportVectorMachineParameters);

	int train() override;

	int test() override;

	int classifyBoard(QVector<QString>& encodings) override;

	int save() override;

	int load() override;

private:
	// get ORB feature from image
	Mat_<float> getFeatureFromImage(
		Mat_<Vec3b> image  // original image
	);

	// based on images, fill X and y
	void getFeaturesAndLabels(
		std::vector<std::pair<Mat_<Vec3b>, std::string>> images,    // training images and their labels
		Mat_<float>& X,											// features matrix. Can differ from class' X, ex: when testing
		Mat_<int>& y											// labels matrix. Can differ from class' y, ex: when testing
	);

	// log a few features as illustration
	void logExampleFeatures();
};