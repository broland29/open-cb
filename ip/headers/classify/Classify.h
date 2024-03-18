#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

#include <spdlog/spdlog.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

#include "KNearestNeighborsClassifier.h"
#include "../EncodingMapperIP.h"

#define TRAIN_SPLIT 0.8

class Classify
{
public:
private:
	AbstractClassifier* classifier;
	std::vector < std::pair<Mat_<Vec3b>, uchar>> trainImages;
	std::vector < std::pair<Mat_<Vec3b>, uchar>> testImages;

public:
	Classify();
	int runTrain();
	int runTest();

private:
};