#include "../../headers/classification/Classifier.h"


Classifier::Classifier(ClassificationParameters classificationParameters)
{
	CNN = new ConvolutionalNeuralNetwork(classificationParameters.convolutionalNeuralNetworkParameters);
	KNN = new KNearestNeighbors(classificationParameters.kNearestNeighborsParameters);
	SVM = new SupportVectorMachine(classificationParameters.supportVectorMachineParameters);
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


int Classifier::saveClassifier()
{
	return activeClassifier->save();
}


int Classifier::loadClassifier()
{
	return activeClassifier->load();
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


void Classifier::updateParameters(ClassificationParameters classificationParameters)
{
	// changes in KNN parameters mean old KNN can be discarded
	if (classificationParameters.kNearestNeighborsParameters.k != KNN->k ||
		classificationParameters.kNearestNeighborsParameters.numberOfBins != KNN->k ||
		classificationParameters.kNearestNeighborsParameters.uniteFrees != KNN->uniteFrees)
	{
		KNN = new KNearestNeighbors(classificationParameters.kNearestNeighborsParameters);
	}

	// changes in SVM parameters mean old SVM can be discarded
	if (classificationParameters.supportVectorMachineParameters.uniteFrees != SVM->uniteFrees)
	{
		SVM = new SupportVectorMachine(classificationParameters.supportVectorMachineParameters);
	}

	// changes in CNN parameters (currently) don't mean that CNN needs to be discarded
	CNN->epochs = classificationParameters.convolutionalNeuralNetworkParameters.epochs;
}
