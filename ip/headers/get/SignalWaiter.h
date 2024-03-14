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
	std::string path;  // where reply from signal will be saved

private:
	CameraReader* cameraReader;
	std::string requestImagePathPrefix;
	std::string signalWaiterName;
	QEventLoop loop;
	QTimer timer;

public:
	SignalWaiter(
		CameraReader* cameraReader,
		std::string requestImagePathPrefix,
		std::string signalWaiterName = "Anonymous"
	);

	int start();

public slots:
	void pathGrabberSlot(QString path);
};