#include "../../headers/get/CameraReader.h"

using namespace cv;


CameraReader::CameraReader(int cameraNo, std::shared_ptr<QMutex> imshowMutex)
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);  // comment out if log if needed
	
	isRunning = false;

	// no need to use mutex in constructor
	configure = false;
	getImage = false;

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
		parameterMutex.lock();
		if (configure)
		{
			sprintf(path, "C:\\open-cb\\mem\\conf_cam%d_cnt%d.jpeg", cameraNo, confCount++);
			imwrite(path, frame);
			
			emit configureSignal(QString::fromLatin1(path));
			
			configure = false;  // "used up"
		}
		if (getImage)
		{
			sprintf(path, "C:\\open-cb\\mem\\get_cam%d_cnt%d.jpeg", cameraNo, getCount++);
			imwrite(path, frame);

			emit getImageSignal(QString::fromLatin1(path));

			getImage = false;  // "used up"
		}
		parameterMutex.unlock();


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

void CameraReader::toggleConfigure()
{
	parameterMutex.lock();
	configure = true;
	parameterMutex.unlock();
}

void CameraReader::toggleGetImage()
{
	parameterMutex.lock();
	getImage = true;
	parameterMutex.unlock();
}