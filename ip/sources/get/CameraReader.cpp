#include "../../headers/get/CameraReader.h"

using namespace cv;


CameraReader::CameraReader(int cameraNo, std::shared_ptr<QMutex> imshowMutex)
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);  // comment out if log if needed
	
	isRunning = false;

	// no need to use mutex in constructor
	requestConfigure = false;
	requestGetImage = false;

	this->cameraNo = cameraNo;
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
	unsigned int confCount = 0;
	unsigned int getCount = 0;

	while (isRunning)
	{
		videoCapture.read(frame);
		if (frame.empty())
		{
			std::cout << "Empty frame!" << std::endl;
			break;
		}

		// check for special requests (before conversion!)
		requestMutex.lock();
		if (requestConfigure)
		{
			sprintf(path, "C:\\open-cb\\mem\\conf_cam%d_cnt%d.jpeg", cameraNo, confCount++);
			imwrite(path, frame);
			
			emit configureDoneSignal(QVariant(QString::fromLatin1(path)));
			
			requestConfigure = false;  // "used up"
		}
		if (requestGetImage)
		{
			sprintf(path, "C:\\open-cb\\mem\\get_cam%d_cnt%d.jpeg", cameraNo, getCount++);
			imwrite(path, frame);

			emit getImageDoneSignal(QVariant(QString::fromLatin1(path)));

			requestGetImage = false;  // "used up"
		}
		requestMutex.unlock();


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

void CameraReader::toggleRequest(std::string request)
{
	requestMutex.lock();
	if (request == "configure")
	{
		requestConfigure = true;
	}
	else if (request == "getImage")
	{
		requestGetImage = true;
	}
	else
	{
		SPDLOG_ERROR("Unknown request {}!", request);
	}
	requestMutex.unlock();
}