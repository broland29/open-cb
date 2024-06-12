#include "../../headers/classification/CxxClassifier.h"


int CxxClassifier::externalToInternal(QString encoding)
{
	if (!uniteFrees)
	{
		if (encoding == "WF") { return 0; };
		if (encoding == "WP") { return 1; };
		if (encoding == "WB") { return 2; };
		if (encoding == "WN") { return 3; };
		if (encoding == "WR") { return 4; };
		if (encoding == "WQ") { return 5; };
		if (encoding == "WK") { return 6; };
		if (encoding == "BF") { return 7; };
		if (encoding == "BP") { return 8; };
		if (encoding == "BB") { return 9; };
		if (encoding == "BN") { return 10; };
		if (encoding == "BR") { return 11; };
		if (encoding == "BQ") { return 12; };
		if (encoding == "BK") { return 13; };
	}
	else  // WF and BF both encoded as 0 ("merged" labels)
	{
		if (encoding == "WF") { return 0; };
		if (encoding == "WP") { return 1; };
		if (encoding == "WB") { return 2; };
		if (encoding == "WN") { return 3; };
		if (encoding == "WR") { return 4; };
		if (encoding == "WQ") { return 5; };
		if (encoding == "WK") { return 6; };
		if (encoding == "BF") { return 0; };
		if (encoding == "BP") { return 7; };
		if (encoding == "BB") { return 8; };
		if (encoding == "BN") { return 9; };
		if (encoding == "BR") { return 10; };
		if (encoding == "BQ") { return 11; };
		if (encoding == "BK") { return 12; };
	}
	
	SPDLOG_ERROR("Could not convert {}, considering it 0", encoding.toStdString());
	return 0;
}


QString CxxClassifier::internalToExternal(int encoding)
{
	if (!uniteFrees)
	{
		if (encoding == 0) { return "WF"; };
		if (encoding == 1) { return "WP"; };
		if (encoding == 2) { return "WB"; };
		if (encoding == 3) { return "WN"; };
		if (encoding == 4) { return "WR"; };
		if (encoding == 5) { return "WQ"; };
		if (encoding == 6) { return "WK"; };
		if (encoding == 7) { return "BF"; };
		if (encoding == 8) { return "BP"; };
		if (encoding == 9) { return "BB"; };
		if (encoding == 10) { return "BN"; };
		if (encoding == 11) { return "BR"; };
		if (encoding == 12) { return "BQ"; };
		if (encoding == 13) { return "BK"; };
	}
	else  // encoding 0 returns WF although it technically contains BF as well. does not matter since VAR does not distinguish
	{
		if (encoding == 0) { return "WF"; };
		if (encoding == 1) { return "WP"; };
		if (encoding == 2) { return "WB"; };
		if (encoding == 3) { return "WN"; };
		if (encoding == 4) { return "WR"; };
		if (encoding == 5) { return "WQ"; };
		if (encoding == 6) { return "WK"; };
		if (encoding == 7) { return "BP"; };
		if (encoding == 8) { return "BB"; };
		if (encoding == 9) { return "BN"; };
		if (encoding == 10) { return "BR"; };
		if (encoding == 11) { return "BQ"; };
	}

	SPDLOG_ERROR("Could not convert {}", encoding);
	return "WF";
}


void CxxClassifier::calculateAndLogMetrics(std::vector<std::vector<int>> confusionMatrix, int testSize)
{
	/* Multiclass classification metrics https://www.evidentlyai.com/classification-metrics/multi-class-metrics
	Global metrics:
	Accuracy: correct predictions / all predictions

	Per class metrics:
	Precision:  correctly classified as C / all classified as C
	Recall:     correctly classified as C / all instances of C

	Can traverse confusion matrix "on the axis", since precision i needs row i, recall i needs column i (and accuracy "needs everything")
	*/
	const int classCount = confusionMatrix.size();
	std::vector<std::string> precisions(classCount);
	std::vector<std::string> recalls(classCount);
	int totalCorrect = 0;

	for (int i = 0; i < classCount; i++)
	{
		int rowSum = 0;  // sum of elements on row i
		int colSum = 0;  // sum of elements on col i
		for (int j = 0; j < classCount; j++)
		{
			rowSum += confusionMatrix[i][j];
			colSum += confusionMatrix[j][i];
		}

		if (colSum == 0)
		{
			precisions[i] = "No correct classifications of class " + std::to_string(i) +
				" (" + std::to_string(confusionMatrix[i][i]) + "/" + std::to_string(colSum) + ")";
		}
		else
		{
			precisions[i] = std::to_string((double)confusionMatrix[i][i] / colSum) +
				" (" + std::to_string(confusionMatrix[i][i]) + "/" + std::to_string(colSum) + ")";
		}
		if (rowSum == 0)
		{
			recalls[i] = "No test images of class " + std::to_string(i) +
				" (" + std::to_string(confusionMatrix[i][i]) + "/" + std::to_string(rowSum) + ")";
		}
		else
		{
			recalls[i] = std::to_string((double)confusionMatrix[i][i] / rowSum) +
				" (" + std::to_string(confusionMatrix[i][i]) + "/" + std::to_string(rowSum) + ")";
		}
		totalCorrect += confusionMatrix[i][i];
	}
	double accuracy = (double)totalCorrect / testSize;

	// get string version to log
	std::string confusionMatrixString;
	for (int i = 0; i < classCount; i++)
	{
		for (int j = 0; j < classCount; j++)
		{
			confusionMatrixString += std::to_string(confusionMatrix[i][j]) + "\t";
		}
		confusionMatrixString += "\n";
	}

	SPDLOG_INFO("Confusion matrix:\n{}", confusionMatrixString);
	for (int i = 0; i < classCount; i++)
	{
		SPDLOG_TRACE("Class {}:\n\tPrecision: {}\n\tRecall: {}", i, precisions[i], recalls[i]);
	}
	SPDLOG_INFO("Accuracy: {}", accuracy);
}
