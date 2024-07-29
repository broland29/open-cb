#pragma once

#include "AbstractClassifier.h"
#include "ConvolutionalNeuralNetwork.h"
#include "KNearestNeighbors.h"
#include "SupportVectorMachine.h"

#include "../com/headers/Parameters.h"


class Classifier
{
public:

private:
	ConvolutionalNeuralNetwork* CNN;
	KNearestNeighbors* KNN;
	SupportVectorMachine* SVM;

	AbstractClassifier* activeClassifier;

public:
	Classifier(ClassificationParameters classificationParameters);

	int changeClassifier(
		std::string newClassifierName  // "KNN", "SVM", "CNN" 
	);

	int saveClassifier();

	int loadClassifier();

	int trainClassifier();

	int testClassifier();

	int classifyBoard(
		QVector<QString>& encodings  // "flattened 8x8 matrix" of encodings will be returned through this parameter
	);

	void updateParameters(
		ClassificationParameters classificationParameters  // parameters with possible changes
	);

private:
};