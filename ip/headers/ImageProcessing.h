#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "get/CameraReader.h"

class ImageProcessing : public QObject
{
	Q_OBJECT

public:
	ImageProcessing();
	static void test();
	CameraReader* cameraReaderOne;
	CameraReader* cameraReaderTwo;
	Crop* cropOne;
	Crop* cropTwo;

signals:
	// right buttons, IP -> UA
	void sendToTrainReplySignal(bool succeeded);
	void sendToTestReplySignal(bool succeeded);
	void runTrainReplySignal(bool succeeded);
	void runTestReplySignal(bool succeeded);
	void resetTrainReplySignal(bool succeeded);
	void resetTestReplySignal(bool succeeded);
	
	// bottom buttons, IP -> UA
	void configureReplySignal(bool succeeded);
	void getImageReplySignal(QString board);

public slots:
	// right buttons, UA -> IP
	void sendToTrainSlot(QString board);
	void sendToTestSlot(QString board);
	void runTrainSlot();
	void runTestSlot();
	void resetTrainSlot();
	void resetTestSlot();

	// bottom buttons, UA -> IP
	void configureSlot();
	void getImageSlot(bool classifyWhenGettingImage);
};