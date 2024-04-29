#pragma once

#include "AbstractClassifier.h"
#include "../CommonIP.h"

#define TRAIN_SCRIPT_PATH "C:\\open-cb\\ip\\sources\\classification\\cnn_train.py"
#define TEST_SCRIPT_PATH "C:\\open-cb\\ip\\sources\\classification\\cnn_test.py"

class ConvolutionalNeuralNetwork : public AbstractClassifier
{
public:
	ConvolutionalNeuralNetwork();

	int train() override;

	int test() override;

	int classifyBoard(QVector<QString>& encodings) override;

	int save(std::string folderPath) override;

	int load(std::string folderPath) override;
};