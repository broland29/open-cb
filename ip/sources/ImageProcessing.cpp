#include "../headers/ImageProcessing.h"

using namespace cv;


ImageProcessing::ImageProcessing()
{
	utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	imshowMutex = std::make_shared<QMutex>();

	cameraReaderOne = new CameraReader(0, imshowMutex);  // modify if needed
	cameraReaderTwo = new CameraReader(1, imshowMutex);  // modify if needed
}


void ImageProcessing::leftToMain(int lr, int lc, int& mr, int& mc)
{
	mr = lc;
	mc = 7 - lr;
}

void ImageProcessing::rightToMain(int rr, int rc, int& mr, int& mc)
{
	mr = 7 - rc;
	mc = rr;
}


// ---------- right buttons, IP -> UA ---------- //

void ImageProcessing::sendToTrainSlot(QString board)
{
	// left part of board detected by cam1, right part by cam2
	// however, each camera sees what's closer to it as "lower cells" (different perspective)
	Mat_<Vec3b> imgCamLeft = imread(PATH_DUMMY_IMG_CAM_ONE, IMREAD_COLOR);

	// todo
	emit sendToTrainReplySignal();
}

void ImageProcessing::sendToTestSlot(QString board)
{
	// todo
	emit sendToTestReplySignal();
}

void ImageProcessing::runTrainSlot()
{
	// todo
	emit runTrainReplySignal();
}

void ImageProcessing::runTestSlot()
{
	// todo
	emit runTestReplySignal();
}

void ImageProcessing::resetTrainSlot()
{
	// todo
	emit resetTrainReplySignal();
}

void ImageProcessing::resetTestSlot()
{
	// todo
	emit resetTestReplySignal();
}


// ---------- bottom buttons, IP -> UA ---------- //

void ImageProcessing::configureSlot()
{
	// request configure and wait for reply signals
	SignalWaiter configureSignalWaiter1{ cameraReaderOne, "conf", "configure1" };
	SignalWaiter configureSignalWaiter2{ cameraReaderTwo, "conf", "configure2" };

	// if any unsuccessful, return
	if (configureSignalWaiter1.start() + configureSignalWaiter2.start() != 0)
	{
		emit configureReplySignal(false, "Error when getting image");
		return;
	}

	// extract paths
	std::string path1 = configureSignalWaiter1.path;
	std::string path2 = configureSignalWaiter2.path;

	Mat_<Vec3b> img1 = imread(path1, IMREAD_COLOR);
	if (img1.empty())
	{
		SPDLOG_ERROR("Image at {} not found", path1);
		emit configureReplySignal(false, "Image not found");
		return;
	}

	Mat_<Vec3b> img2 = imread(path2, IMREAD_COLOR);
	if (img2.empty())
	{
		SPDLOG_ERROR("Image at {} not found", path2);
		emit configureReplySignal(false, "Image not found");
		return;
	}

	if (Crop::configure(img1, corners1, configured1, imshowMutex) + Crop::configure(img2, corners2, configured2, imshowMutex) != 0)
	{
		emit configureReplySignal(false, "Error when cropping");
		return;
	}

	imshowMutex->lock();
	imshow(path1, img1);
	imshow(path2, img2);
	waitKey();
	imshowMutex->unlock();

	emit configureReplySignal();
}


void ImageProcessing::getImageSlot(bool classifyWhenGettingImage)
{
	// note: reply message starting with "$" reserved to encode that a board is being passed

	// request getImage and wait for reply signals
	SignalWaiter getImageSignalWaiter1{ cameraReaderOne, "get", "getImage1" };
	SignalWaiter getImageSignalWaiter2{ cameraReaderTwo, "get", "getImage2" };

	// if any unsuccessful, return
	if (getImageSignalWaiter1.start() + getImageSignalWaiter2.start() != 0)
	{
		emit getImageReplySignal(false, "Error when getting image");
		return;
	}

	// extract paths
	std::string path1 = getImageSignalWaiter1.path;
	std::string path2 = getImageSignalWaiter2.path;

	Mat_<Vec3b> img1 = imread(path1, IMREAD_COLOR);
	if (img1.empty())
	{
		SPDLOG_ERROR("Image at {} not found", path1);
		emit getImageReplySignal("Camera one image not found");
		return;
	}

	Mat_<Vec3b> img2 = imread(path2, IMREAD_COLOR);
	if (img2.empty())
	{
		SPDLOG_ERROR("Image at {} not found", path2);
		emit getImageReplySignal("Camera two image not found");
		return;
	}

	if (classifyWhenGettingImage)
	{
		// TODO: call classification
		QString board = "$";

		imshowMutex->lock();
		imshow(path1, img1);
		imshow(path2, img2);
		waitKey();
		imshowMutex->unlock();
	
		emit getImageReplySignal(true, board);
		return;
	}
	else
	{
		imshowMutex->lock();
		imshow(path1, img1);
		imshow(path2, img2);
		waitKey();
		imshowMutex->unlock();

		emit getImageReplySignal();
		return;
	}
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