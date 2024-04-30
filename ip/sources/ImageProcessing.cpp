#include "../headers/ImageProcessing.h"


ImageProcessing::ImageProcessing()
{
	utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	// fill parameters
	ParametersHandler::loadFromJSON(parameters);

	imshowMutex = std::make_shared<QMutex>();

	setupCameraHandlerLeft();
	setupCameraHandlerRight();

	classifier = new Classifier();
	configurerLeft = new Configurer(CameraSide::LEFT, imshowMutex);
	configurerRight = new Configurer(CameraSide::RIGHT, imshowMutex);
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


void ImageProcessing::saveClassifierSlot()
{
	int ret = classifier->saveClassifier();
	if (ret != 0)
	{
		emit saveClassifierReplySignal(false, "Saving classifier failed");
		return;
	}
	emit saveClassifierReplySignal(true, "Saved classifier.");
}


void ImageProcessing::loadClassifierSlot()
{
	int ret = classifier->loadClassifier();
	if (ret != 0)
	{
		emit loadClassifierReplySignal(false, "Loading classifier failed");
		return;
	}
	emit loadClassifierReplySignal(true, "Loaded classifier.");
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
	// get images
	Mat_<Vec3b> imgLeft, imgRight;
	if (getImage(new SignalWaiter(cameraHandlerLeft, "clas", "classifyLeft"), imgLeft) != 0)
	{
		emit classifyBoardReplySignal(false, "Error getting image from left camera", QVector<QString>());
		return;
	}
	if (getImage(new SignalWaiter(cameraHandlerRight, "clas", "classifyRight"), imgRight) != 0)
	{
		emit classifyBoardReplySignal(false, "Error getting image from right camera", QVector<QString>());
		return;
	}

	// call on configurer - prepare cell images
	const int borderTop = get<int>(parameters.parameters["borderTop"]);
	const int borderRight = get<int>(parameters.parameters["borderRight"]);
	const int borderBottom = get<int>(parameters.parameters["borderBottom"]);
	const int borderLeft = get<int>(parameters.parameters["borderLeft"]);
	if (configurerLeft->prepareCellImages(imgLeft, borderTop, borderRight, borderBottom, borderLeft) +
		configurerRight->prepareCellImages(imgRight, borderTop, borderRight, borderBottom, borderLeft) != 0)
	{
		emit classifyBoardReplySignal(false, "Preparing cell images failed. Make sure to run \"Configure\" beforehand.", QVector<QString>());
		return;
	}

	// call on classifier
	QVector<QString> encodings;
	if (classifier->classifyBoard(encodings) != 0)
	{
		emit classifyBoardReplySignal(false, "Classifying board failed. Make sure to run \"Train\" beforehand.", QVector<QString>());
		return;
	}

	emit classifyBoardReplySignal(true, "Classification successful", encodings);
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
	if (configurerLeft->configure(imgLeft, isTest, get<bool>(parameters.parameters["showImages"]), get<bool>(parameters.parameters["concatImages"])) +
	    configurerRight->configure(imgRight, isTest, get<bool>(parameters.parameters["showImages"]), get<bool>(parameters.parameters["concatImages"])) != 0)
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


void ImageProcessing::cropAndLabel(QVector<QString> encodings, bool isTest)
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
	const bool concatImages = get<bool>(parameters.parameters["concatImages"]);
	const int borderTop = get<int>(parameters.parameters["borderTop"]);
	const int borderRight = get<int>(parameters.parameters["borderRight"]);
	const int borderBottom = get<int>(parameters.parameters["borderBottom"]);
	const int borderLeft = get<int>(parameters.parameters["borderLeft"]);
	if (configurerLeft->cropAndLabel(imgLeft, encodings, isTest, concatImages, borderTop, borderRight, borderBottom, borderLeft) +
		configurerRight->cropAndLabel(imgRight, encodings, isTest, concatImages, borderTop, borderRight, borderBottom, borderLeft) != 0)
	{
		emit cropAndLabelReplySignal(false, "Error when configuring");
		return;
	}

	cropAndLabelReplySignal(true, "Configuration successful");
}

void ImageProcessing::testCropAndLabelSlot(QVector<QString> encodings)
{
	cropAndLabel(encodings, true);
}

void ImageProcessing::cropAndLabelSlot(QVector<QString> encodings)
{
	cropAndLabel(encodings, false);
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


void ImageProcessing::setParametersSlot(QVector<QString> names, QVector<QString> values)
{
	if (names.length() != values.length())
	{
		SPDLOG_ERROR("Size mismatch");
		return;
	}

	// camera indices are special: they are used "non-stop" by camera handlers; so even though parameter gets updated
	//   in parameters.parameters, we need to "update" the camera readers to see immediate effect
	int oldLeftCameraIndex = get<int>(parameters.parameters["leftCameraIndex"]);
	int oldRightCameraIndex = get<int>(parameters.parameters["rightCameraIndex"]);

	parameters.setValues(names, values);
	ParametersHandler::saveToJSON(parameters);

	int newLeftCameraIndex = get<int>(parameters.parameters["leftCameraIndex"]);
	int newRightCameraIndex = get<int>(parameters.parameters["rightCameraIndex"]);
	if (newLeftCameraIndex != oldLeftCameraIndex)
	{
		SPDLOG_TRACE("Switching left camera index from {} to {}", oldLeftCameraIndex, newLeftCameraIndex);
		setupCameraHandlerLeft();
	}
	if (newRightCameraIndex != oldRightCameraIndex)
	{
		SPDLOG_TRACE("Switching right camera index from {} to {}", oldRightCameraIndex, newRightCameraIndex);
		setupCameraHandlerRight();
	}

	emit setParametersReplySignal(true, "Modifications saved");
}


void ImageProcessing::getParametersSlot(QVector<QString> names)
{
	ParametersHandler::loadFromJSON(parameters);

	QVector<QString> values(names.size());
	parameters.getValues(names, values);
	
	emit getParametersReplySignal(names, values);
}


// https://stackoverflow.com/questions/8165487/how-to-do-cleaning-up-on-exit-in-qt
void ImageProcessing::beforeQuit()
{
	SPDLOG_TRACE("Saving parameters");
	ParametersHandler::saveToJSON(parameters);
	SPDLOG_TRACE("Parameters saved");
}


void ImageProcessing::setupCameraHandlerLeft()
{
	// if exists and running, exit, so cleanup happens
	if (cameraHandlerLeftThread != NULL && cameraHandlerLeftThread->isRunning())
	{
		cameraHandlerLeft->stop();
	}

	// replace thread and handler
	cameraHandlerLeftThread = new QThread;
	cameraHandlerLeft = new CameraHandler(get<int>(parameters.parameters["leftCameraIndex"]));

	// thread connections
	QObject::connect(cameraHandlerLeftThread, &QThread::started, cameraHandlerLeft, &CameraHandler::doWork);
	QObject::connect(cameraHandlerLeft, &CameraHandler::stop, cameraHandlerLeftThread, &QThread::quit, Qt::BlockingQueuedConnection);
	QObject::connect(cameraHandlerLeftThread, &QThread::finished, cameraHandlerLeft, &CameraHandler::deleteLater);

	// imageProcessing connections - delegate
	QObject::connect(cameraHandlerLeft, &CameraHandler::previewImageReadySignal, this, &ImageProcessing::previewImageReadyLeftSignal, Qt::BlockingQueuedConnection);

	// move to thread and start
	cameraHandlerLeft->moveToThread(cameraHandlerLeftThread);
	cameraHandlerLeftThread->start();
}


void ImageProcessing::setupCameraHandlerRight()
{
	// if exists and running, exit, so cleanup happens
	if (cameraHandlerRightThread != NULL && cameraHandlerRightThread->isRunning())
	{
		cameraHandlerRight->stop();
	}

	// replace thread and handler
	cameraHandlerRightThread = new QThread;
	cameraHandlerRight = new CameraHandler(get<int>(parameters.parameters["rightCameraIndex"]));

	// thread connections
	QObject::connect(cameraHandlerRightThread, &QThread::started, cameraHandlerRight, &CameraHandler::doWork);
	QObject::connect(cameraHandlerRight, &CameraHandler::stop, cameraHandlerRightThread, &QThread::quit, Qt::BlockingQueuedConnection);
	QObject::connect(cameraHandlerRightThread, &QThread::finished, cameraHandlerRight, &CameraHandler::deleteLater);

	// imageProcessing connections - delegate
	QObject::connect(cameraHandlerRight, &CameraHandler::previewImageReadySignal, this, &ImageProcessing::previewImageReadyRightSignal, Qt::BlockingQueuedConnection);

	// move to thread and start
	cameraHandlerRight->moveToThread(cameraHandlerRightThread);
	cameraHandlerRightThread->start();
}


void ImageProcessing::previewImageReadyLeftSlot(QImage image)
{
	emit previewImageReadyLeftSignal(image);
}


void ImageProcessing::previewImageReadyRightSlot(QImage image)
{
	emit previewImageReadyRightSignal(image);
}