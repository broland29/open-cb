#pragma once

#include "AbstractClassifier.h"
#include "../CommonIP.h"
#include <fstream>

#define TRAIN_SCRIPT_PATH "C:\\open-cb\\ip\\sources\\classification\\cnn_train.py"
#define TEST_SCRIPT_PATH "C:\\open-cb\\ip\\sources\\classification\\cnn_test.py"
#define CLASSIFY_BOARD_SCRIPT_PATH "C:\\open-cb\\ip\\sources\\classification\\cnn_classify_board.py"

class ConvolutionalNeuralNetwork : public AbstractClassifier
{
public:

private:

public:
	ConvolutionalNeuralNetwork();

	int train() override;

	int test() override;

	int classifyBoard(QVector<QString>& encodings) override;

	int save() override;

	int load() override;

private:
	// map internal encoding (0, 1, ...) to external (see ENCODINGS in Common.h)
	QString internalToExternal(int encoding);
};