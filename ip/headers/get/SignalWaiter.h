#pragma once

#include <QObject>
#include <QVariant>
#include <QEventLoop>
#include <QTimer>
#include <string>

#include "CameraReader.h"



class SignalWaiter : public QObject
{
	Q_OBJECT

public:
	QVariant variant;  // where reply from signal will be saved

private:
	CameraReader* cameraReader;
	std::string request;
	std::string signalWaiterName;
	QEventLoop loop;
	QTimer timer;

public:
	SignalWaiter(
		CameraReader* cameraReader,
		std::string request,
		std::string signalWaiterName = "Anonymous"
	);

	int start();

public slots:
	void variantGrabberSlot(QVariant variant);
};