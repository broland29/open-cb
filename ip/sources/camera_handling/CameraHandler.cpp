#include "../../headers/camera_handling/CameraHandler.h"


CameraHandler::CameraHandler(int index)
{
	this->index = index;
	isRunning = false;
	isImageRequested = false;  // no need to use mutex in constructor
}


void CameraHandler::doWork()
{
	VideoCapture videoCapture(index, cv::CAP_DSHOW);  // with this option it is faster

	if (!videoCapture.isOpened())
	{
		SPDLOG_ERROR("Could not open camera {}!", index);
		return;
	}

	Mat_<Vec3b> frame;
	unsigned int count = 0;

	isRunning = true;
	int emptyFrameTolerance = 3;
	while (isRunning)
	{
		videoCapture.read(frame);
		if (frame.empty())
		{
			SPDLOG_TRACE("Got empty frame, camera {}!", index);
			//break;
			waitKey(30);
			continue;
			/*
			emptyFrameTolerance--;
			if (emptyFrameTolerance > 0)
			{
				waitKey(30);
				continue;  // try reading another
			}
			else
			{
				break;
			}
			*/
		}

		// check for requests (before conversion!)
		isImageRequestedMutex.lock();
		if (isImageRequested)
		{
			std::string path;
			int ret = FileHandler::saveImage(frame, GRAB_FOLDER_PATH, path, true);  // don't care about naming, temporary anyways
			if (ret != 0)
			{
				emit requestImageReplySignal(false, "Grabbing image failed!");
				continue;
			}
			emit requestImageReplySignal(true, QString::fromLatin1(path));
			
			// "used up"
			isImageRequested = false;
			requestImagePathPrefix = "";
		}
		isImageRequestedMutex.unlock();

		cv::cvtColor(frame, frame, COLOR_BGR2RGB);
		if (isRunning)
		{
			emit previewImageReadySignal(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888));
		}

		waitKey(30); 
		// for waitKey(5), sometimes I got "Access violation reading location" in MainWindow::imageUpdateSlotOne
		// maybe because 5 milliseconds is not enough to process signals -> doWork ends -> thread ends??
	}
}


void CameraHandler::stop()
{
	isRunning = false;
}


void CameraHandler::requestImage(std::string requestImagePathPrefix)
{
	isImageRequestedMutex.lock();
	isImageRequested = true;
	this->requestImagePathPrefix = requestImagePathPrefix;
	isImageRequestedMutex.unlock();
}