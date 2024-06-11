#include "../../headers/file_handling/ParametersHandler.h"


// https://github.com/nlohmann/json?tab=readme-ov-file#simplify-your-life-with-macros
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	CameraHandlerParameters,
	leftCameraIndex,
	rightCameraIndex
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	CameraHandlingParameters,
	cameraHandlerParameters
)

/*
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	ClassificationParameters,
	kNearestNeighborsParameters,
	supportVectorMachineParameters,
	convolutionalNeuralNetworkParameters
)
*/

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	ConfigureParameters,
	gaussianFilterDimension,
	binaryThreshold,
	closingFilterDimension,
	houghRoStepSize,
	houghThetaStepSize,	
	houghWindowSize,
	houghNumberOfLines,
	showImages,
	concatImages
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	BorderParameters,
	borderTop,
	borderRight,
	borderBottom,
	borderLeft
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	CropAndLabelParameters,
	showImages,
	concatImages
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	ConfigurationParameters,
	configureParameters,
	borderParameters,
	cropAndLabelParameters
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	Parameters,
	cameraHandlingParameters,
	//classificationParameters,
	configurationParameters//,
	//fileHandlingParameters
)

int ParametersHandler::loadFromJSON(Parameters& parameters)
{
	SPDLOG_INFO("Before loading: {}", parameters);
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
		SPDLOG_ERROR("Error when reading/parsing {}. See exception below.", JSON_PATH);
		SPDLOG_ERROR(ex.what());
		return 2;
	}

	try
	{
		parameters = jsonBuffer;  // implicitly uses from_json defined by macros
	}
	catch (json::exception& ex)
	{
		SPDLOG_ERROR("Error when converting jsonBuffer to parameters. See exception below.");
		SPDLOG_ERROR(ex.what());
		return 3;
	}

	SPDLOG_INFO("After loading: {}", parameters);
	
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

	json jsonBuffer = parameters; // implicitly uses to_json defined by macros

	SPDLOG_INFO("Will write to file:\n{}", jsonBuffer.dump(4));

	outfile << std::setw(4) << jsonBuffer;

	return 0;
}
