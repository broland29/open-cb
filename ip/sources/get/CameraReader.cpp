#include "../../headers/get/CameraReader.h"

using namespace cv;


CameraReader::CameraReader(int cameraNo, std::shared_ptr<QMutex> imshowMutex)
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);  // comment out if log if needed
	isRunning = false;
	this->cameraNo = cameraNo;
	this->saveAFrame = false;
	this->imshowMutex = imshowMutex;
}


void CameraReader::doWork()
{
	isRunning = true;

	VideoCapture videoCapture(cameraNo, cv::CAP_DSHOW);  // with this option it is faster
	//VideoCapture videoCapture(cameraNo, cv::CAP_);

	if (!videoCapture.isOpened())
	{
		SPDLOG_ERROR("Could not open camera {}!", cameraNo);
		return;
	}

	Mat_<Vec3b> frame;
	char path[256];
	sprintf(path, "C:\\open-cb\\mem\\cam%d.jpeg", cameraNo);

	while (isRunning)
	{
		videoCapture.read(frame);
		if (frame.empty())
		{
			std::cout << "Empty frame!" << std::endl;
			break;
		}

		// if save requested, do before conversion!
		saveAFrameMutex.lock();
		if (saveAFrame)
		{
			imwrite(path, frame);

			imshowMutex.get()->lock();
			imshow(path, frame);
			Crop::crop(frame);

			waitKey();
			imshowMutex.get()->unlock();

			saveAFrame = false;
		}
		saveAFrameMutex.unlock();

		cv::cvtColor(frame, frame, COLOR_BGR2RGB);
		if (isRunning)
		{
			emit imageUpdateSignal(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888));
		}

		waitKey(30); 
		// for waitKey(5), sometimes I got "Access violation reading location" in MainWindow::imageUpdateSlotOne
		// maybe because 5 milliseconds is not enough to process signals -> doWork ends -> thread ends??
	}
}

void CameraReader::stop()
{
	isRunning = false;
}

void CameraReader::deleteLater()
{

}

void CameraReader::setSaveAFrame(bool value)
{
	saveAFrameMutex.lock();
	saveAFrame = value;
	saveAFrameMutex.unlock();
}

bool CameraReader::getSaveAFrame()
{
	return saveAFrame;
}