#include "../../headers/get/SignalWaiter.h"


SignalWaiter::SignalWaiter(CameraReader* cameraReader, std::string requestImagePathPrefix, std::string signalWaiterName)
{
	this->cameraReader = cameraReader;
	this->requestImagePathPrefix = requestImagePathPrefix;
	this->signalWaiterName = signalWaiterName;

	// timer which will give time limit so no infinite wait occurs
	timer.setSingleShot(true);

	// connect the reply signal with the pathGrabberSlot, so that in success case transmitted parameter gets saved
	QObject::connect(cameraReader, &CameraReader::imageSaved, this, &SignalWaiter::pathGrabberSlot);
	
	// if the timer times out, loop quits
	QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
}


int SignalWaiter::start()
{
	timer.start(3000);  // milliseconds

	// not a problem if signal gets emitted before entering loop.exec(),
	//   since pathGrabberSlot catches it (loop is just for timer)
	cameraReader->toggleRequest(requestImagePathPrefix);

	loop.exec();

	if (timer.isActive())  // exited loop but timer is still active => did not timeout
	{
		return 0;
	}

	SPDLOG_ERROR("Signal waiter {} timed out!", signalWaiterName);
	return 1;
}


void SignalWaiter::pathGrabberSlot(QString path)
{
	this->path = path.toStdString();	// grab path
	loop.exit();						// exit waiting loop
}
