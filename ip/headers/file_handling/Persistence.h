#pragma once

#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <nlohmann/json.hpp>
#include "../Parameters.h"


using json = nlohmann::json;

#define PATH "C:\\open-cb\\mem\\per\\persistence.json"



class Persistence
{
public:
	Parameters parameters;

private:

public:
	Persistence();
	void save();
	friend void to_json(json& j, const Parameters& p);
	friend void from_json(const json& j, Parameters& p);

private:
};
