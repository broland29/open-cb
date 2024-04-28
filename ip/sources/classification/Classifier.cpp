#include "../../headers/classification/Classifier.h"


Classifier::Classifier()
{
	CNN = new ConvolutionalNeuralNetwork();
	KNN = new KNearestNeighbors();
	SVM = new SupportVectorMachine();
	activeClassifier = KNN;
}


int Classifier::changeClassifier(std::string newClassifierName)
{
	if (newClassifierName == "CNN")
	{
		activeClassifier = CNN;
		return 0;
	}
	if (newClassifierName == "KNN")
	{
		activeClassifier = KNN;
		return 0;
	}
	if (newClassifierName == "SVM")
	{
		activeClassifier = SVM;
		return 0;
	}

	SPDLOG_ERROR("Unknown classifier name {}!", newClassifierName);
	return 1;
}


int Classifier::saveClassifier(std::string folderPath)
{
	return activeClassifier->save(folderPath);
}


int Classifier::loadClassifier(std::string folderPath)
{
	return activeClassifier->load(folderPath);
}


int Classifier::trainClassifier()
{
	return activeClassifier->train();
}


int Classifier::testClassifier()
{
	return activeClassifier->test();
}


int Classifier::classifyBoard(QVector<QString>& encodings)
{
	return activeClassifier->classifyBoard(encodings);
}