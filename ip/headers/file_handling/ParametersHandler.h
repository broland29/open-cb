#pragma once

#include "../Parameters.h"
#include "../CommonIP.h"

#include <fstream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <QVector>
#include <QString>


using json = nlohmann::json;


class ParametersHandler
{
	// no fields

public:
	// load parameters from PATH, if not found, do nothing
	static int loadFromJSON(Parameters& parameters);

	// save parameters to PATH
	static int saveToJSON(Parameters parameters);

private:
};
