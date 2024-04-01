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
	while (isRunning)
	{
		videoCapture.read(frame);
		if (frame.empty())
		{
			SPDLOG_TRACE("Got empty frame, camera {}!", index);
			break;
		}

		// check for requests (before conversion!)
		isImageRequestedMutex.lock();
		if (isImageRequested)
		{
			std::string path =
				"C:\\open-cb\\mem\\get\\" +				// main folder
				requestImagePathPrefix +				// whatever the requester wants to add to the name
				"_cam" + std::to_string(index) +		// the camera's index
				"_cnt" + std::to_string(count) +		// the number of image
				".jpeg";								// extension
			
			// imwrite and error handling - https://docs.opencv.org/3.4/d4/da8/group__imgcodecs.html#gabbc7ef1aa2edfaa87772f1202d67e0ce
			int result = false;
			try
			{
				result = imwrite(path, frame);
			}
			catch (const cv::Exception& ex)
			{
				SPDLOG_ERROR("Conversion for {} failed: exception {}, camera {}!", path, ex.what(), index);
				isImageRequested = false;
				requestImagePathPrefix = "";
				emit requestImageReplySignal(false, "Conversion failed!");
				isImageRequestedMutex.unlock();
				continue;
			}

			if (!result)
			{
				SPDLOG_ERROR("Saving for {} failed, camera {}!", path, index);
				isImageRequested = false;
				requestImagePathPrefix = "";
				emit requestImageReplySignal(false, "Saving failed!");
				isImageRequestedMutex.unlock();
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