#pragma once

#include <map>
#include <sstream>
#include <spdlog/spdlog.h>

#include "AbstractClassifier.h"
#include "../file_handling/FileHandler.h"


class CxxClassifier : public AbstractClassifier
{
public:
	bool uniteFrees;  // parameter

private:

protected:
	const bool DEBUG = true;

public:

private:

protected:
	bool trained;

	// map external encoding ("WF", "WP", ...) to internal (0, 1, ...)
	int externalToInternal(
		QString encoding
	);

	// map internal encoding (0, 1, ...) to external ("WF", "WP", ...)
	QString internalToExternal(
		int encoding
	);

	void logDistribution(
		std::map<std::string, int> labelsAndCounts,			// number of images grouped by labels
		std::string setType									// "train", "validation", "test"
	);
};