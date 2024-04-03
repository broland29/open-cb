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

	virtual int train() override;

	virtual int test() override;

	virtual std::string classifyBoard() override;

	virtual int save(std::string folderPath) override;

	virtual int load(std::string folderPath) override;
};