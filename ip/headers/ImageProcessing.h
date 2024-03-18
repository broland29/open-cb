#pragma once

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

#include <QTimer>
#include <QEventLoop>
#include <QObject>
#include <QVariant>

#include "get/CameraReader.h"
#include "get/SignalWaiter.h"
#include "EncodingMapperIP.h"
#include "classify/Classify.h"

class ImageProcessing : public QObject
{
	Q_OBJECT

public:
	CameraReader* cameraReaderOne;
	CameraReader* cameraReaderTwo;

private:
	// fields related to camera one
	std::vector<Point2f> corners1;		// corners of chessboard
	bool configured1;					// true if corners set at least once

	// fields related to camera two
	std::vector<Point2f> corners2;
	bool configured2;

	// common fields
	std::shared_ptr<QMutex> imshowMutex;
	unsigned int count;  // the count of images which were saved; common since success case only when both cameras save

	Classify* classify = NULL;
public:
	ImageProcessing();
	static void test();

private:
	void sendToFolder(
		QString board,			// the board we get from signal
		std::string folder		// train or test
	);

signals:
	// right buttons, IP -> UA
	void sendToTrainReplySignal(bool succeeded = true, QString message = "Success");
	void sendToTestReplySignal(bool succeeded = true, QString message = "Success");
	void runTrainReplySignal(bool succeeded = true, QString message = "Success");
	void runTestReplySignal(bool succeeded = true, QString message = "Success");
	void resetTrainReplySignal(bool succeeded = true, QString message = "Success");
	void resetTestReplySignal(bool succeeded = true, QString message = "Success");
	
	// bottom buttons, IP -> UA
	void configureReplySignal(bool succeeded = true, QString message = "Success");
	void getImageReplySignal(bool succeeded = true, QString message = "Success");

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