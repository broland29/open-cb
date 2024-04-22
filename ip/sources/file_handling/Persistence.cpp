#include "../../headers/file_handling/Persistence.h"
#include <iostream>

void to_json(json& j, const Parameters& p)
{
	j = json
	{
		{"leftCameraIndex", p.leftCameraIndex},
		{"rightCameraIndex", p.rightCameraIndex}
	};
}


void from_json(const json& j, Parameters& p)
{
	j.at("leftCameraIndex").get_to(p.leftCameraIndex);
	j.at("rightCameraIndex").get_to(p.rightCameraIndex);
}


std::ostream& operator<<(std::ostream& os, const Parameters& parameters)
{
	os << "leftCameraIndex: " << parameters.leftCameraIndex << std::endl;
	os << "rightCameraIndex: " << parameters.rightCameraIndex << std::endl;
	return os;
}


Persistence::Persistence()
{
	std::ifstream infile(PATH);
	
	// initially default values for "parameters"
	if (!infile.is_open())
	{
		SPDLOG_INFO("Could not open infile {}, using default parameters", PATH);
		return;
	}

	json buffer;
	infile >> buffer;
	parameters = buffer.template get<::Parameters>();

	SPDLOG_TRACE("Loaded parameters:\n{}", parameters);
}


void Persistence::save()
{
	std::ofstream outfile(PATH);
	if (!outfile.is_open())
	{
		SPDLOG_ERROR("Could not open outfile {}", PATH);
		return;
	}

	// write to file
	json buffer = parameters;
	outfile << std::setw(4) << buffer << std::endl;
}






