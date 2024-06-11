#include "../headers/Parameters.h"


std::ostream& operator<<(std::ostream& os, const Parameters& parameters)
{
	os << std::endl << "cameraHandlerParameters:" << std::endl;
	os << "    leftCameraIndex: " << parameters.cameraHandlingParameters.cameraHandlerParameters.leftCameraIndex << std::endl;
	os << "    rightCameraIndex: " << parameters.cameraHandlingParameters.cameraHandlerParameters.rightCameraIndex << std::endl;

	os << std::endl << "configureParameters:" << std::endl;
	os << "    gaussianFilterDimension: " << parameters.configurationParameters.configureParameters.gaussianFilterDimension << std::endl;
	os << "    binaryThreshold: " << static_cast<int>(parameters.configurationParameters.configureParameters.binaryThreshold) << std::endl;
	os << "    closingFilterDimension: " << parameters.configurationParameters.configureParameters.closingFilterDimension << std::endl;
	os << "    houghRoStepSize: " << parameters.configurationParameters.configureParameters.houghRoStepSize << std::endl;
	os << "    houghThetaStepSize: " << parameters.configurationParameters.configureParameters.houghThetaStepSize << std::endl;
	os << "    houghWindowSize: " << parameters.configurationParameters.configureParameters.houghWindowSize << std::endl;
	os << "    houghNumberOfLines: " << parameters.configurationParameters.configureParameters.houghNumberOfLines << std::endl;
	os << "    showImages: " << parameters.configurationParameters.configureParameters.showImages << std::endl;
	os << "    concatImages: " << parameters.configurationParameters.configureParameters.concatImages << std::endl;

	os << std::endl << "borderParameters:" << std::endl;
	os << "    borderTop: " << parameters.configurationParameters.borderParameters.borderTop << std::endl;
	os << "    borderRight: " << parameters.configurationParameters.borderParameters.borderRight << std::endl;
	os << "    borderBottom: " << parameters.configurationParameters.borderParameters.borderBottom << std::endl;
	os << "    borderLeft: " << parameters.configurationParameters.borderParameters.borderLeft << std::endl;

	os << std::endl << "cropAndLabelParameters:" << std::endl;
	os << "    showImages: " << parameters.configurationParameters.cropAndLabelParameters.showImages << std::endl;
	os << "    concatImages: " << parameters.configurationParameters.cropAndLabelParameters.concatImages << std::endl;

	return os;
}