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
	int classCount;				// uniteFrees ? 13 : 14
	bool trained;				// true if train was called
	const bool DEBUG = true;	// toggle extra logging

public:
	CxxClassifier(bool uniteFrees);

private:

protected:

	// map FileHandler::labelFolderNames to internal encoding (0, 1, ...)
	int externalToInternal(
		std::string encoding
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