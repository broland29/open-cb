#pragma once

#include "../CommonIP.h"
#include "../file_handling/FileHandler.h"

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QObject>
#include <chrono>
#include <ctime>


class CameraHandler : public QObject
{
	Q_OBJECT

public:

private:
	int index;
	bool isRunning;
	bool isImageRequested;  // https://stackoverflow.com/questions/4897912/how-to-properly-interrupt-a-qthread-infinite-loop
	std::string requestImagePathPrefix;
	QMutex isImageRequestedMutex;

public:
	CameraHandler(
		int index  // the camera index, as used in VideoCapture constructor
	);

	// thread loop
	void doWork();
	
	// exit thread loop
	void stop();
	
	// request an image to be saved on the disk
	void requestImage(
		std::string requestImagePathPrefix = ""  // optional short string to be part of path, for debugging later
	);

signals:
	void previewImageReadySignal(
		QImage previewImage  // no success field needed. sent only if successful, without a request
	);

	void requestImageReplySignal(
		bool succeeded,
		QString message  // path / error message
	);
};