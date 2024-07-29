#include "../../headers/camera_handling/SignalWaiter.h"


SignalWaiter::SignalWaiter(CameraHandler* cameraHandler, std::string requestImagePathPrefix, std::string signalWaiterName)
{
	this->cameraHandler = cameraHandler;
	this->requestImagePathPrefix = requestImagePathPrefix;
	this->signalWaiterName = signalWaiterName;

	// timer which will give time limit so no infinite wait occurs
	timer.setSingleShot(true);

	// connect the reply signal with the pathGrabberSlot, so that in success case transmitted parameter gets saved
	QObject::connect(cameraHandler, &CameraHandler::requestImageReplySignal, this, &SignalWaiter::requestImageReplySlot);
	
	// if the timer times out, loop quits
	QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
}


int SignalWaiter::start()
{
	timer.start(3000);  // milliseconds

	// not a problem if signal gets emitted before entering loop.exec(),
	//   since pathGrabberSlot catches it (loop is just for timer)
	cameraHandler->requestImage(requestImagePathPrefix);

	loop.exec();

	if (timer.isActive())  // exited loop but timer is still active => did not timeout
	{
		return 0;
	}

	SPDLOG_ERROR("Signal waiter {} timed out!", signalWaiterName);
	return 1;
}


void SignalWaiter::requestImageReplySlot(bool succeeded, QString message)
{
	this->succeeded = succeeded;
	this->message = message.toStdString();
	loop.exit();  // exit waiting loop
}
