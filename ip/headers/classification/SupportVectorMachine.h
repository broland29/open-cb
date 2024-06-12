#pragma once

#include "CxxClassifier.h"
#include "KNearestNeighbors.h"
#include "../file_handling/FileHandler.h"
#include "../Common.h"
#include "../Parameters.h"

#include <opencv2/xfeatures2d.hpp>
#include <opencv2/ml.hpp>


#define N_FEATURES		500
#define SCALE_FACTOR	1.2f
#define N_LEVELS		8
#define EDGE_THRESHOLD	15					// default 31
#define FIRST_LEVEL		0
#define WTA_K			2
#define SCORE_TYPE		ORB::HARRIS_SCORE
#define PATCH_SIZE		31
#define FAST_THRESHOLD	20

class SupportVectorMachine : public CxxClassifier
{
public:

private:
	std::vector<KeyPoint> keyPoints;
	Ptr<ORB> orb;
	Ptr<ml::SVM> svm;

	Mat_<float> X;      // feature matrix	flattened ORB descriptors matrix							float for svm to accept it
	Mat_<int> y;        // class labels		0, 1, ..., see internalToExternal, externalToInternal		int for svm to accept it

public:
	SupportVectorMachine(SupportVectorMachineParameters supportVectorMachineParameters);

	int train() override;

	int test() override;

	int classifyBoard(QVector<QString>& encodings) override;

	int save() override;

	int load() override;

private:
	Mat_<float> getFeatureFromImage(Mat_<Vec3b> image);

	void getFeaturesAndLabels(std::vector<std::pair<Mat_<Vec3b>, QString>> images, Mat_<float>& X, Mat_<int>& y);

	void logExampleFeatures();

	inline std::string getPath() { return SVM_FOLDER_PATH + std::string("\\svm.txt"); }
};