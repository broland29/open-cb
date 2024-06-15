#pragma once

#include <fstream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <QVector>
#include <QString>

#include "Parameters.h"
#include "Paths.h"


using json = nlohmann::json;


class ParametersHandler
{
public:

private:

public:
	// load parameters from PATH, if not found, do nothing
	static int loadFromJSON(Parameters& parameters);

	// save parameters to PATH
	static int saveToJSON(Parameters parameters);

private:
};
