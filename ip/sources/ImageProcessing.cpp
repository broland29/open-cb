#include "../headers/ImageProcessing.h"


ImageProcessing::ImageProcessing()
{
	utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	imshowMutex = std::make_shared<QMutex>();

	cameraHandlerLeft = new CameraHandler(leftCameraIndex);
	cameraHandlerRight = new CameraHandler(rightCameraIndex);
	classifier = new Classifier();
	configurerLeft = new Configurer(CameraSide::LEFT);
	configurerRight = new Configurer(CameraSide::RIGHT);
}


int ImageProcessing::getImage(SignalWaiter* signalWaiter, Mat_<Vec3b>& img)
{
	// if unsuccessful, return
	if (signalWaiter->start() != 0)
	{
		SPDLOG_ERROR("Error when getting image");
		return 1;
	}
	if (signalWaiter->succeeded == false)
	{
		SPDLOG_ERROR("Failure grabbed, message {}", signalWaiter->message);
		return 2;
	}

	// extract image
	std::string path = signalWaiter->message;
	img = imread(path, IMREAD_COLOR);
	if (img.empty())
	{
		SPDLOG_ERROR("Image at {} not found", path);
		return 3;
	}
	
	return 0;
}


void ImageProcessing::changeClassifierSlot(QString newClassifierName)
{
	int ret = classifier->changeClassifier(newClassifierName.toStdString());
	if (ret != 0)
	{
		emit changeClassifierReplySignal(false, "Changing classifier failed");
		return;
	}
	emit changeClassifierReplySignal(true, "Changed classifier");
}


void ImageProcessing::saveClassifierSlot(QString folderPath)
{
	int ret = classifier->saveClassifier(folderPath.toStdString());
	if (ret != 0)
	{
		emit saveClassifierReplySignal(false, "Saving classifier failed");
		return;
	}
	emit saveClassifierReplySignal(true, "Saved classifier to " + folderPath);
}


void ImageProcessing::loadClassifierSlot(QString folderPath)
{
	int ret = classifier->loadClassifier(folderPath.toStdString());
	if (ret != 0)
	{
		emit loadClassifierReplySignal(false, "Loading classifier failed");
		return;
	}
	emit loadClassifierReplySignal(true, "Loaded classifier from " + folderPath);
}


void ImageProcessing::trainClassifierSlot()
{
	int ret = classifier->trainClassifier();
	if (ret != 0)
	{
		emit trainClassifierReplySignal(false, "Training classifier failed");
		return;
	}
	emit trainClassifierReplySignal(true, "Trained classifier");
}


void ImageProcessing::testClassifierSlot()
{
	int ret = classifier->testClassifier();
	if (ret != 0)
	{
		emit testClassifierReplySignal(false, "Testing classifier failed");
		return;
	}
	emit testClassifierReplySignal(true, "Testing classifier");
}


void ImageProcessing::classifyBoardSlot()
{
	// todo - should map. from classifyBoard() it gets a string 

	// get images
	Mat_<Vec3b> imgLeft, imgRight;
	if (getImage(new SignalWaiter(cameraHandlerLeft, "clas", "classifyLeft"), imgLeft) != 0)
	{
		emit classifyBoardReplySignal(false, "Error getting image from left camera");
		return;
	}
	if (getImage(new SignalWaiter(cameraHandlerRight, "clas", "classifyRight"), imgRight) != 0)
	{
		emit classifyBoardReplySignal(false, "Error getting image from right camera");
		return;
	}

	// call on configurer - prepare cell images
	if (configurerLeft->prepareCellImages(imgLeft) + configurerRight->prepareCellImages(imgRight) != 0)
	{
		emit configureReplySignal(false, "Error when preparing cells");
		return;
	}

	// call on classifier
	std::string ret = classifier->classifyBoard();
	if (ret == "")
	{
		emit classifyBoardReplySignal(false, "Classifying board failed");
		return;
	}

	emit classifyBoardReplySignal(true, QString::fromStdString(ret));
}


void ImageProcessing::configure(bool isTest)
{
	// get images
	Mat_<Vec3b> imgLeft , imgRight;
	if (getImage(new SignalWaiter(cameraHandlerLeft, "conf", "configureLeft"), imgLeft) != 0)
	{
		emit configureReplySignal(false, "Error getting image from left camera");
		return;
	}
	if (getImage(new SignalWaiter(cameraHandlerRight, "conf", "configureRight"), imgRight) != 0)
	{
		emit configureReplySignal(false, "Error getting image from right camera");
		return;
	}
	
	// call on configurer
	if (configurerLeft->configure(imgLeft, isTest) + configurerRight->configure(imgRight, isTest) != 0)
	{
		emit configureReplySignal(false, "Error when configuring");
		return;
	}

	emit configureReplySignal(true, "Configuration successful");
}

void ImageProcessing::testConfigureSlot()
{
	configure(true);
}

void ImageProcessing::configureSlot()
{
	configure(false);
}


void ImageProcessing::cropAndLabel(std::string board[64], bool isTest)
{
	// get images
	Mat_<Vec3b> imgLeft, imgRight;
	if (getImage(new SignalWaiter(cameraHandlerLeft, "crop", "cropAndLabelLeft"), imgLeft) != 0)
	{
		emit cropAndLabelReplySignal(false, "Error getting image from left camera");
		return;
	}
	if (getImage(new SignalWaiter(cameraHandlerRight, "crop", "cropAndLabelRight"), imgRight) != 0)
	{
		emit cropAndLabelReplySignal(false, "Error getting image from right camera");
		return;
	}

	// call on configurer
	if (configurerLeft->cropAndLabel(imgLeft, board, isTest) + configurerRight->cropAndLabel(imgRight, board, isTest) != 0)
	{
		emit cropAndLabelReplySignal(false, "Error when configuring");
		return;
	}

	cropAndLabelReplySignal(true, "Configuration successful");
}

void ImageProcessing::testCropAndLabelSlot(QString board)
{
	std::string _board[64];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			EncodingMapperIP::map(board[i * 8 + j], i, j, _board[i * 8 + j]);
		}
	}
	cropAndLabel(_board, true);
}

void ImageProcessing::cropAndLabelSlot(QString board)
{
	std::string _board[64];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			EncodingMapperIP::map(board[i * 8 + j], i, j, _board[i * 8 + j]);
		}
	}
	cropAndLabel(_board, false);
}


void ImageProcessing::shuffleAndSplitSlot()
{
	if (FileHandler::shuffleAndSplit() != 0)
	{
		emit shuffleAndSplitReplySignal(false, "Error shuffling and splitting!");
		return;
	}
	emit shuffleAndSplitReplySignal(true, "Shuffled and splitted successfully!");
}

void ImageProcessing::clearAllImagesSlot()
{
	if (FileHandler::clearAllImages() != 0)
	{
		emit clearAllImagesReplySignal(false, "Clearing all images failed!");
		return;
	}
	emit clearAllImagesReplySignal(true, "Cleared all images successfully!");
}

void ImageProcessing::changeSettingsSlot()
{
	// todo - set specific parameters of specific submodule which changed
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