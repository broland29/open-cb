#pragma once

#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


// just like in opencv2/core/hal/interface.h
typedef unsigned char uchar;



// ---------- package camera_handling ---------- //

struct CameraHandlerParameters
{
	int leftCameraIndex = 0;
	int rightCameraIndex = 1;
};

struct CameraHandlingParameters
{
	CameraHandlerParameters cameraHandlerParameters;
};


// ---------- package classification ---------- //

struct KNearestNeighborsParameters
{
	int k = 5;
	int numberOfBins = 8;
	bool uniteFrees = true;
};

struct SupportVectorMachineParameters
{
	bool uniteFrees = false;
};

struct ConvolutionalNeuralNetworkParameters
{
	int epochs = 20;
	bool applyAugmentation = true;
};

struct ClassificationParameters
{
	KNearestNeighborsParameters kNearestNeighborsParameters;
	SupportVectorMachineParameters supportVectorMachineParameters;
	ConvolutionalNeuralNetworkParameters convolutionalNeuralNetworkParameters;
};


// ---------- package configuration ---------- //

struct ConfigureParameters
{
	int gaussianFilterDimension = 7;		// width/height of the gaussian kernel
	uchar binaryThreshold = 120;			// < binaryThreshold ? 0 : 255
	int closingFilterDimension = 6;			// width/heoght of the closing kernel
	int houghRoStepSize = 1;				// size of one increment in ro when searching for lines	(granulaarity)
	int houghThetaStepSize = 1;				// size of one increment in theta when searching for lines (granularity)
	int houghWindowSize = 3;				// area in which local maxima is searched for
	int houghNumberOfLines = 10;			// maximum number of lines (if less found, less returned)
	bool showImages = true;
	bool concatImages = true;
};

struct BorderParameters
{
	int borderTop = 53;
	int borderRight = 50;
	int borderBottom = 55;
	int borderLeft = 70;
};
struct CropAndLabelParameters
{
	bool showImages = true;
	bool concatImages = true;
};

struct ConfigurationParameters
{
	ConfigureParameters configureParameters;
	BorderParameters borderParameters;
	CropAndLabelParameters cropAndLabelParameters;
};


// ---------- package file handling ---------- //

struct FileHandlerParameters
{
	double trainSplit = 0.6;
	double validationSplit = 0.2;
	double testSplit = 0.2;
};

struct FileHandlingParameters
{
	FileHandlerParameters fileHandlerParameters;
};


// ---------- big wrapper ---------- //

struct Parameters
{
	CameraHandlingParameters cameraHandlingParameters;
	ClassificationParameters classificationParameters;
	ConfigurationParameters configurationParameters;
	FileHandlingParameters fileHandlingParameters;

	friend std::ostream& operator<<(std::ostream& os, const Parameters& parameters);
};
#if FMT_VERSION >= 90000
template <> struct fmt::formatter<Parameters> : ostream_formatter {};
#endif
