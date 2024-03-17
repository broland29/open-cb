#include "../headers/ImageProcessing.h"

using namespace cv;


ImageProcessing::ImageProcessing()
{
	utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	imshowMutex = std::make_shared<QMutex>();

	cameraReaderOne = new CameraReader(0, CameraSide::LEFT, imshowMutex);  // modify if needed
	cameraReaderTwo = new CameraReader(1, CameraSide::RIGHT, imshowMutex);  // modify if needed

	count = 0;  // todo: store in memory, session-wise
}


// ---------- right buttons, IP -> UA ---------- //

// https://isocpp.org/wiki/faq/pointers-to-members
// https://stackoverflow.com/questions/1485983/how-can-i-create-a-pointer-to-a-member-function-and-call-it
typedef void (ImageProcessing::* ReplySignal)(bool succeeded, QString message);
#define EMIT_REPLY_SIGNAL(replySignal, succeeded, message) (*this.*replySignal)(succeeded, message)

void ImageProcessing::sendToFolder(QString board, std::string folder)
{
	ReplySignal replySignal = (board == "train") ? &ImageProcessing::sendToTrainReplySignal : &ImageProcessing::sendToTestReplySignal;

	// request image and wait for response
	SignalWaiter configureSignalWaiter1{ cameraReaderOne, folder + "_temp", folder + "1" };
	SignalWaiter configureSignalWaiter2{ cameraReaderTwo, folder + "_temp", folder + "2" };

	// if any unsuccessful, return
	if (configureSignalWaiter1.start() + configureSignalWaiter2.start() != 0)
	{
		EMIT_REPLY_SIGNAL(replySignal, false, "Error when getting image");
		return;
	}
	count++;

	// extract paths
	std::string path1 = configureSignalWaiter1.path;
	std::string path2 = configureSignalWaiter2.path;

	Mat_<Vec3b> img1 = imread(path1, IMREAD_COLOR);
	if (img1.empty())
	{
		SPDLOG_ERROR("Image at {} not found", path1);
		EMIT_REPLY_SIGNAL(replySignal, false, "Image not found");
		return;
	}

	Mat_<Vec3b> img2 = imread(path2, IMREAD_COLOR);
	if (img2.empty())
	{
		SPDLOG_ERROR("Image at {} not found", path2);
		EMIT_REPLY_SIGNAL(replySignal, false, "Image not found");
		return;
	}

	std::string board_[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (EncodingMapperIP::map(board[i * 8 + j], i, j, board_[i][j]) != 0)
			{
				EMIT_REPLY_SIGNAL(replySignal, false, "Error when processing board");
				return;
			}
		}
	}

	if (Crop::sendToTrain(img1, corners1, configured1, cameraReaderOne->cameraSide, board_, count) +
		Crop::sendToTrain(img2, corners2, configured2, cameraReaderTwo->cameraSide, board_, count) != 0)
	{
		EMIT_REPLY_SIGNAL(replySignal, false, "Error when cropping");
		return;
	}

	EMIT_REPLY_SIGNAL(replySignal, true, "Success!");
}

void ImageProcessing::sendToTrainSlot(QString board)
{
	sendToFolder(board, "train");
}

void ImageProcessing::sendToTestSlot(QString board)
{
	sendToFolder(board, "test");
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
	if (Crop::resetTrain() != 0)
	{
		emit resetTrainReplySignal(false, "Reset train failed");
		return;
	}

	emit resetTrainReplySignal();
}

void ImageProcessing::resetTestSlot()
{
	if (Crop::resetTest() != 0)
	{
		emit resetTestReplySignal(false, "Reset test failed");
		return;
	}

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
	count++;

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
	count++;

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
		if (Crop::getImage(img1, corners1, configured1, imshowMutex) +
			Crop::getImage(img2, corners2, configured2, imshowMutex) != 0)
		{
			emit configureReplySignal(false, "Error when cropping");
			return;
		}

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