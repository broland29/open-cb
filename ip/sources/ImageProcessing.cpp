#include "../headers/ImageProcessing.h"

using namespace cv;


ImageProcessing::ImageProcessing()
{
	utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	std::shared_ptr<QMutex> imshowMutex = std::make_shared<QMutex>();

	cameraReaderOne = new CameraReader(0, imshowMutex);  // modify if needed
	cameraReaderTwo = new CameraReader(1, imshowMutex);  // modify if needed
	
	cropOne = new Crop(imshowMutex);
	cropTwo = new Crop(imshowMutex);

	QObject::connect(cameraReaderOne, &CameraReader::configureSignal, cropOne, &Crop::configureSlot);
	QObject::connect(cameraReaderTwo, &CameraReader::configureSignal, cropTwo, &Crop::configureSlot);
	QObject::connect(cameraReaderOne, &CameraReader::getImageSignal, cropOne, &Crop::getImageSlot);
	QObject::connect(cameraReaderTwo, &CameraReader::getImageSignal, cropTwo, &Crop::getImageSlot);
}


// ---------- right buttons, IP -> UA ---------- //

void ImageProcessing::sendToTrainSlot(QString board)
{
	// todo
	emit sendToTrainReplySignal(true);
}

void ImageProcessing::sendToTestSlot(QString board)
{
	// todo
	emit sendToTestReplySignal(true);
}

void ImageProcessing::runTrainSlot()
{
	// todo
	emit runTrainReplySignal(true);
}

void ImageProcessing::runTestSlot()
{
	// todo
	emit runTestReplySignal(true);
}

void ImageProcessing::resetTrainSlot()
{
	// todo
	emit resetTrainReplySignal(true);
}

void ImageProcessing::resetTestSlot()
{
	// todo
	emit resetTestReplySignal(true);
}


// ---------- bottom buttons, IP -> UA ---------- //

void ImageProcessing::configureSlot()
{
	cameraReaderOne->toggleConfigure();
	cameraReaderTwo->toggleConfigure();

	emit configureReplySignal(true);
}

void ImageProcessing::getImageSlot(bool classiftWhenGettingImage)
{
	cameraReaderOne->toggleGetImage();
	cameraReaderTwo->toggleGetImage();

	if (classiftWhenGettingImage)
	{
		// todo
	}
	QString predictedBoard = "todo";
	emit getImageReplySignal(predictedBoard);
}



void ImageProcessing::test()
{
	std::cout << "Hello from ImageProcessing!" << std::endl;

	Mat_<Vec3b> img = imread("C:\\open-cb\\img.png", IMREAD_COLOR);
	if (img.empty())
	{
		std::cout << "Could not open img.png" << std::endl;
		return;
	}

	imshow("img.png", img);
	waitKey();
}