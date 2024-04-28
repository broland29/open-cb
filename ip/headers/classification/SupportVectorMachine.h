#pragma once

#include "AbstractClassifier.h"
#include "../Common.h"
#include "../file_handling/FileHandler.h"

#include <opencv2/xfeatures2d.hpp>
#include <opencv2/ml.hpp>


class SupportVectorMachine : public AbstractClassifier
{
public:

private:
	Ptr<ml::SVM> svm;

public:
	SupportVectorMachine();

	int train() override;

	int test() override;

	int classifyBoard(QVector<QString>& encodings) override;

	int save(std::string folderPath) override;

	int load(std::string folderPath) override;
};