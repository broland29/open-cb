#include "../headers/ParametersHandler.h"


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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	KNearestNeighborsParameters,
	k,
	numberOfBins,
	uniteFrees
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	SupportVectorMachineParameters,
	uniteFrees
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	ConvolutionalNeuralNetworkParameters,
	epochs,
	applyAugmentation
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	ClassificationParameters,
	kNearestNeighborsParameters,
	supportVectorMachineParameters,
	convolutionalNeuralNetworkParameters
)

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
	FileHandlerParameters,
	trainSplit,
	validationSplit,
	testSplit
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	FileHandlingParameters,
	fileHandlerParameters
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
	Parameters,
	cameraHandlingParameters,
	classificationParameters,
	configurationParameters,
	fileHandlingParameters
)

int ParametersHandler::loadFromJSON(Parameters& parameters)
{
	SPDLOG_INFO("Before loading: {}", parameters);
	std::ifstream infile(Paths::JSON_PARAMETERS);

	if (!infile.is_open())
	{
		SPDLOG_INFO("Could not open infile {}", Paths::JSON_PARAMETERS);
		return 1;
	}

	json jsonBuffer;
	try
	{
		infile >> jsonBuffer;
	}
	catch (json::exception& ex)
	{
		SPDLOG_ERROR("Error when reading/parsing {}. See exception below.", Paths::JSON_PARAMETERS);
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
	std::ofstream outfile(Paths::JSON_PARAMETERS);

	if (!outfile)
	{
		SPDLOG_INFO("Could not open outfile {}", Paths::JSON_PARAMETERS);
		return 1;
	}

	json jsonBuffer = parameters; // implicitly uses to_json defined by macros

	SPDLOG_INFO("Will write to file:\n{}", jsonBuffer.dump(4));

	outfile << std::setw(4) << jsonBuffer;

	return 0;
}
