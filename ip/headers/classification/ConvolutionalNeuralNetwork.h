#pragma once

#include "AbstractClassifier.h"
#include "../Common.h"

#define TRAIN_SCRIPT_PATH "C:\\open-cb\\ip\\sources\\classification\\train.py"

class ConvolutionalNeuralNetwork : public AbstractClassifier
{
public:
	ConvolutionalNeuralNetwork();

	virtual int train() override;

	virtual int test() override;

	std::string classifyBoard() override;

	virtual int save(std::string folderPath) override;

	virtual int load(std::string folderPath) override;
};