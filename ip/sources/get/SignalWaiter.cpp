#include "../../headers/get/SignalWaiter.h"


SignalWaiter::SignalWaiter(CameraReader* cameraReader, std::string request, std::string signalWaiterName)
{
	this->cameraReader = cameraReader;
	this->request = request;
	this->signalWaiterName = signalWaiterName;

	// timer which will give time limit so no infinite wait occurs
	timer.setSingleShot(true);

	// connect the reply signal with the variantGrabberSlot, so that in success case transmitted parameter gets saved
	if (request == "configure")
	{
		QObject::connect(cameraReader, &CameraReader::configureDoneSignal, this, &SignalWaiter::variantGrabberSlot);
	}
	else if (request == "getImage")
	{
		QObject::connect(cameraReader, &CameraReader::getImageDoneSignal, this, &SignalWaiter::variantGrabberSlot);
	}
	else
	{
		SPDLOG_ERROR("Unknown request {}!", request);  // will be just a useless timer
	}
	
	// if the timer times out, loop quits
	connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
}


int SignalWaiter::start()
{
	timer.start(3000);  // milliseconds

	// not a problem if signal gets emitted before entering loop.exec(),
	//   since variantGrabberSlot catches it (loop is just for timer)
	cameraReader->toggleRequest(request);

	loop.exec();

	if (timer.isActive())  // exited loop but timer is still active => did not timeout
	{
		return 0;
	}

	SPDLOG_ERROR("Signal waiter {} timed out!", signalWaiterName);
	return 1;
}


void SignalWaiter::variantGrabberSlot(QVariant variant)
{
	this->variant = variant;	// grab variant
	loop.exit();				// exit waiting loop
}
