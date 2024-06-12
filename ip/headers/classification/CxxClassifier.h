#pragma once

#include "AbstractClassifier.h"
#include "../Common.h"
#include "../file_handling/FileHandler.h"


class CxxClassifier : public AbstractClassifier
{
public:
	bool debug;
	bool uniteFrees;

protected:
	bool trained;

	// map external encoding ("WF", "WP", ...) to internal (0, 1, ...)
	int externalToInternal(QString encoding);

	// map internal encoding (0, 1, ...) to external ("WF", "WP", ...)
	QString internalToExternal(int encoding);

	void calculateAndLogMetrics(std::vector<std::vector<int>> confusionMatrix, int testSize);
};