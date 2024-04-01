#pragma once

#include "AbstractClassifier.h"
#include "ConvolutionalNeuralNetwork.h"
#include "KNearestNeighbors.h"
#include "SupportVectorMachine.h"

#include <array>


class Classifier
{
public:
	Classifier();

private:
	ConvolutionalNeuralNetwork* CNN;
	KNearestNeighbors* KNN;
	SupportVectorMachine* SVM;

	AbstractClassifier* activeClassifier;

public:
	int changeClassifier(std::string newClassifierName);
	int saveClassifier(std::string folderPath);
	int loadClassifier(std::string folderPath);
	int trainClassifier();
	int testClassifier();
	std::string classifyBoard();

private:
};