#pragma once

#include "CameraHandler.h"

#include <string>

#include <QObject>
#include <QVariant>
#include <QEventLoop>
#include <QTimer>


class SignalWaiter : public QObject
{
	Q_OBJECT

public:
	// what's saved from reply signal
	bool succeeded;
	std::string message;  // path / error message

private:
	CameraHandler* cameraHandler;			// camera handler to request from
	std::string requestImagePathPrefix;		// to pass to camera handler request
	std::string signalWaiterName;			// additional debug info
	
	QEventLoop loop;
	QTimer timer;

public:
	SignalWaiter(
		CameraHandler* cameraHandler,
		std::string requestImagePathPrefix,
		std::string signalWaiterName = "Anonymous"
	);

	// make request, wait for response
	int start();

public slots:
	// grabs the path
	void requestImageReplySlot(bool succeeded, QString message);
};