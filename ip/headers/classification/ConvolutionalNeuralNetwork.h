#pragma once

#include <fstream>

#include "AbstractClassifier.h"


class ConvolutionalNeuralNetwork : public AbstractClassifier
{
public:
	int epochs;  // parameter
	bool applyAugmentation;  // parameter

private:

public:
	ConvolutionalNeuralNetwork(ConvolutionalNeuralNetworkParameters convolutionalNeuralNetworkParameters);

	int train() override;

	int test() override;

	int classifyBoard(QVector<QString>& encodings) override;

	int save() override;

	int load() override;

private:
	// map internal encoding (0, 1, ...) to external ("WF", "WP", ...) based on how script encodes!
	QString internalToExternal(int encoding);
};