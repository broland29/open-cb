#include "../../headers/classification/AbstractClassifier.h"


void AbstractClassifier::calculateAndLogMetrics(std::vector<std::vector<int>> confusionMatrix, std::vector<std::string> encodings)
{
	/* Multiclass classification metrics https://www.evidentlyai.com/classification-metrics/multi-class-metrics

	For us:
	TP = true positive = correctly classified as C
	TN = true negative = correctly not classified as C. not of interest here
	FP = false positive = incorrectly classified as C
	FN = false negative = incorrectly not classified as C

	Global metric:
	Accuracy = correct predictions / all predictions

	Class-level metrics:
	Precision = TP / (TP + FP) = correctly classified as C / all classified as C
	Recall    = TP / (TP + FN) = correctly classified as C / all instances of C

	Macro-average:
	Precision = sum(precisions) / classNo = average precision, no/same weights for each class
	Recall    = sum(recalls) / classNo    = average recall, no/same weights for each class

	Micro-average:
	Precision =	sum(TPs) / sum(TPs + FPs) = weighted average precision
	Recall	  = sum(TPs) / sum(TPs + FNs) = weighted average recall


	So what we need:
	correct predictions, all predictions
	all TPs, FPs, FNs
	all presisions, recalls and number of classes

	For us, confusionMatrix[actualClass][predictedClass], so
	correct predictions = sum on first axis
	all predictions = sum of whole confusion matrix
	TP(c) =		actualClass == c && predictedClass == c		= element on first axis
	FP(c) =		actualClass != c && predictedClass == c		= elements on column c, excluding the one on first axis
	FN(c) =		actualClass == c && predictedClass != c		= elements on row x, excluding the one on first axis
	precisions and recalls come from this, just need to store; number of classes is the width/height of confusion matrix
	*/

	const int classCount = confusionMatrix.size();

	int correctPredictionCount = 0;
	int allPredictionCount = 0;
	std::vector<double> TPs(classCount);
	std::vector<double> FPs(classCount);
	std::vector<double> FNs(classCount);
	for (int i = 0; i < classCount; i++)
	{
		for (int j = 0; j < classCount; j++)
		{
			allPredictionCount += confusionMatrix[i][j];
			if (i == j)
			{
				correctPredictionCount += confusionMatrix[i][j];
				TPs[i] = confusionMatrix[i][j];
			}
			else
			{
				FPs[j] += confusionMatrix[i][j];
				FNs[i] += confusionMatrix[i][j];
			}
		}
	}
	double accuracy = (double)correctPredictionCount / allPredictionCount;

	std::vector<double> precisions(classCount);
	std::vector<double> recalls(classCount);
	for (int i = 0; i < classCount; i++)
	{
		precisions[i] = (TPs[i] + FPs[i] == 0) ? 0 : TPs[i] / (TPs[i] + FPs[i]);
		recalls[i] = (TPs[i] + FNs[i] == 0) ? 0 : TPs[i] / (TPs[i] + FNs[i]);
	}

	double sumPrecisions = 0;
	double sumRecalls = 0;
	double sumTPs = 0;
	double sumFPs = 0;
	double sumFNs = 0;
	for (int i = 0; i < classCount; i++)
	{
		sumPrecisions += precisions[i];
		sumRecalls += recalls[i];
		sumTPs += TPs[i];
		sumFPs += FPs[i];
		sumFNs += FNs[i];
	}

	double macroPrecision = sumPrecisions / classCount;
	double macroRecall = sumRecalls / classCount;
	double microPrecision = (sumTPs + sumFPs == 0) ? 0 : sumTPs / (sumTPs + sumFPs);
	double microRecall = (sumTPs + sumFNs == 0) ? 0 : sumTPs / (sumTPs + sumFNs);

	// now log everything
	std::ostringstream os;
	os << "Confusion matrix" << std::endl;
	for (int i = 0; i < classCount; i++)
	{
		for (int j = 0; j < classCount; j++)
		{
			os << std::setfill(' ') << std::setw(5) << confusionMatrix[i][j] << " ";
		}
		os << std::endl;
	}
	os << std::endl;
	os << "Accuracy: " << accuracy << std::endl;

	os << std::endl;
	os << std::setw(5) << "Class" << std::setw(15) << "Precision" << std::setw(15) << "Recall" << std::endl;
	for (int i = 0; i < classCount; i++)
	{
		os << std::setfill(' ') << std::setw(5) << encodings[i] << std::fixed << std::setprecision(4) << std::setw(15) << precisions[i] << std::setw(15) << recalls[i] << std::endl;
	}

	os << std::endl;
	os << "Macro precision: " << macroPrecision << std::endl;
	os << "Macro recall: " << macroRecall << std::endl;
	os << "Micro precision: " << microPrecision << std::endl;
	os << "Micro recall: " << microRecall << std::endl;

	SPDLOG_INFO(os.str());
}