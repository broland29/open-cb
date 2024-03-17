#include "../../headers/get/CameraReader.h"

using namespace cv;


CameraReader::CameraReader(int cameraNo, CameraSide cameraSide, std::shared_ptr<QMutex> imshowMutex)
{
	this->cameraNo = cameraNo;
	this->cameraSide = cameraSide;
	this->imshowMutex = imshowMutex;

	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);  // comment out if log if needed
	
	isRunning = false;
	isImageRequested = false;  		// no need to use mutex in constructor
	requestImagePathPrefix = "";
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
	unsigned int count = 0;

	while (isRunning)
	{
		videoCapture.read(frame);
		if (frame.empty())
		{
			std::cout << "Empty frame!" << std::endl;
			break;
		}

		// check for requests (before conversion!)
		requestMutex.lock();
		if (isImageRequested)
		{
			std::string path =
				"C:\\open-cb\\mem\\get\\" +				// main folder
				requestImagePathPrefix +				// whatever the requester wants to add to the name
				"_cam" + std::to_string(cameraNo) +		// the camera's number
				"_cnt" + std::to_string(count) +		// the number of image
				".jpeg";								// extension
			
			// imwrite and error handling
			int result = false;
			try
			{
				result = imwrite(path, frame);
			}
			catch (const cv::Exception& ex)
			{
				SPDLOG_ERROR("Exception converting image: {}!", ex.what());
				isImageRequested = false;
				requestImagePathPrefix = "";
				emit imageSaved("CONVERSION_ERROR_PATH");  // todo - add success to signal
				requestMutex.unlock();
				continue;
			}

			if (!result)
			{
				SPDLOG_ERROR("Could not save image!");
				isImageRequested = false;
				requestImagePathPrefix = "";
				emit imageSaved("SAVE_ERROR_PATH");
				requestMutex.unlock();
				continue;
			}
			
			emit imageSaved(QString::fromLatin1(path));
			
			// "used up"
			isImageRequested = false;
			requestImagePathPrefix = "";
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

void CameraReader::toggleRequest(std::string requestImagePathPrefix)
{
	requestMutex.lock();
	isImageRequested = true;
	this->requestImagePathPrefix = requestImagePathPrefix;
	requestMutex.unlock();
}