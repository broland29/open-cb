#include "../../headers/file_handling/ParametersHandler.h"


int ParametersHandler::loadFromJSON(Parameters& parameters)
{
	SPDLOG_INFO("Before loading: {}", parameters.parameters);
	std::ifstream infile(JSON_PATH);

	if (!infile.is_open())
	{
		SPDLOG_INFO("Could not open infile {}", JSON_PATH);
		return 1;
	}

	json jsonBuffer;
	try
	{
		infile >> jsonBuffer;
	}
	catch (json::exception& ex)
	{
		SPDLOG_ERROR("Possible error when parsing {}. See exception below.", JSON_PATH);
		SPDLOG_ERROR(ex.what());
		return 2;
	}

	// iterate a copy of map. names the same, values change, types of values shall be the same
	Parameters oldParameters = parameters;
	for (const auto& pair : oldParameters.parameters)
	{
		std::string name = pair.first.toStdString();
		Variant oldValue = pair.second;

		if (!jsonBuffer.contains(name))
		{
			SPDLOG_ERROR("{} does not contain key {}", JSON_PATH, name);
			return 3;
		}

		// extract new value, knowing type from old. exception thrown if types not matching
		Variant newValue;
		try
		{
			if (std::holds_alternative<bool>(oldValue))
			{
				newValue = jsonBuffer[name].template get<bool>();
			}
			else if (std::holds_alternative<int>(oldValue))
			{
				newValue = jsonBuffer[name].template get<int>();
			}
			else if (std::holds_alternative<double>(oldValue))
			{
				newValue = jsonBuffer[name].template get<double>();
			}
			else if (std::holds_alternative<std::string>(oldValue))
			{
				newValue = jsonBuffer[name].template get<std::string>();
			}
			else
			{
				SPDLOG_ERROR("Unsupported type");
				return 4;
			}
		}
		catch (json::exception& e)
		{
			SPDLOG_ERROR("Possible mismatch of old and new value for {}. See exception below.", name);
			SPDLOG_ERROR(e.what());
		}

		// update map
		parameters.setValue(pair.first, newValue);
	}

	SPDLOG_INFO("After loading: {}", parameters.parameters);
	return 0;
}


int ParametersHandler::saveToJSON(Parameters parameters)
{
	std::ofstream outfile(JSON_PATH);

	if (!outfile)
	{
		SPDLOG_INFO("Could not open outfile {}", JSON_PATH);
		return 1;
	}

	json jsonBuffer;
	for (const auto& pair : parameters.parameters)  // iterate map
	{
		std::string name = pair.first.toStdString();
		Variant value = pair.second;

		if (std::holds_alternative<bool>(value))
		{
			jsonBuffer[name] = std::get<bool>(value);
		}
		else if (std::holds_alternative<int>(value))
		{
			jsonBuffer[name] = std::get<int>(value);
		}
		else if (std::holds_alternative<double>(value))
		{
			jsonBuffer[name] = std::get<double>(value);
		}
		else if (std::holds_alternative<std::string>(value))
		{
			jsonBuffer[name] = std::get<std::string>(value);
		}
		else
		{
			SPDLOG_ERROR("Unsupported type");
			return 2;
		}
	}

	SPDLOG_INFO("Will write to file:\n{}", jsonBuffer.dump(4));
	outfile << std::setw(4) << jsonBuffer;

	return 0;
}
