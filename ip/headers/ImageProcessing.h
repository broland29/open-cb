#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <QTimer>
#include <QEventLoop>
#include <QObject>
#include "get/CameraReader.h"
#include <QVariant>
#include "get/SignalWaiter.h"

class ImageProcessing : public QObject
{
	Q_OBJECT

public:
	CameraReader* cameraReaderOne;
	CameraReader* cameraReaderTwo;

private:
	// fields related to camera one
	std::vector<Point2f> corners1;
	bool configured1;

	// fields related to camera two
	std::vector<Point2f> corners2;
	bool configured2;

	// common fields
	std::shared_ptr<QMutex> imshowMutex;

public:
	ImageProcessing();
	static void test();

private:
	// coordinate system transformation: left -> main (see cams.drawio)
	void leftToMain(int lr, int lc, int& mr, int& mc);

	// coordinate system transformation: right -> main (see cams.drawio)
	void rightToMain(int rr, int rc, int& mr, int& mc);

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