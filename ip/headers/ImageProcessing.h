#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "CameraReader.h"

class ImageProcessing
{
public:
	ImageProcessing();
	static void test();
	CameraReader* cameraReaderOne;
	CameraReader* cameraReaderTwo;
};