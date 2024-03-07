#include "../headers/CameraReader.h"

using namespace cv;


CameraReader::CameraReader(int cameraNo)
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);  // comment out if log if needed
	isRunning = false;
	this->cameraNo = cameraNo;
}


void CameraReader::doWork()
{
	isRunning = true;

	VideoCapture videoCapture(cameraNo, cv::CAP_DSHOW);  // with this option it is faster

	if (!videoCapture.isOpened())
	{
		SPDLOG_ERROR("Could not open camera {}!", cameraNo);
		return;
	}

	Mat_<Vec3b> frame;
	while (isRunning)
	{
		videoCapture.read(frame);
		if (frame.empty())
		{
			std::cout << "Empty frame!" << std::endl;
			break;
		}

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