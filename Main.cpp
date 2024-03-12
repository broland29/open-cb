#include "log/headers/Logger.h"
#include <iostream>
#include "ua/headers/UserApplication.h"
#include "ip/headers/ImageProcessing.h"
#include "var/headers/ValidationAndResponse.h"

#define TEST_DEPENDENCIES false


// here only connect's which are inter-module
int main(int argc, char* argv[])
{
	Logger::initialize();

	if (TEST_DEPENDENCIES)
	{
		ImageProcessing::test();
		ValidationAndResponse::test();
		return UserApplication::test(argc, argv);
	}

	ImageProcessing* imageProcessing = new ImageProcessing();
	ValidationAndResponse* validationAndResponse = new ValidationAndResponse();
	UserApplication* userApplication = new UserApplication(argc, argv);

	// for shorter code
	CameraReader* cameraReaderOne = imageProcessing->cameraReaderOne;
	CameraReader* cameraReaderTwo = imageProcessing->cameraReaderTwo;
	Crop* cropOne = imageProcessing->cropOne;
	Crop* cropTwo = imageProcessing->cropTwo;
	MainWindow* mainWindow = userApplication->mainWindow;

	// create worker threads
	QThread* cameraReaderOneThread = new QThread;
	QObject::connect(cameraReaderOneThread, &QThread::started, cameraReaderOne, &CameraReader::doWork);
	QObject::connect(cameraReaderOne, &CameraReader::stop, cameraReaderOneThread, &QThread::quit);
	QObject::connect(cameraReaderOneThread, &QThread::finished, cameraReaderOne, &CameraReader::deleteLater);

	QThread* cameraReaderTwoThread = new QThread;
	QObject::connect(cameraReaderTwoThread, &QThread::started, cameraReaderTwo, &CameraReader::doWork);
	QObject::connect(cameraReaderTwo, &CameraReader::stop, cameraReaderTwoThread, &QThread::quit);
	QObject::connect(cameraReaderTwoThread, &QThread::finished, cameraReaderTwo, &CameraReader::deleteLater);

	QThread* cropOneThread = new QThread;
	QThread* cropTwoThread = new QThread;

	// cross - thread communication
	QObject::connect(cameraReaderOne, &CameraReader::imageUpdateSignal, mainWindow, &MainWindow::imageUpdateSlotOne);
	QObject::connect(cameraReaderTwo, &CameraReader::imageUpdateSignal, mainWindow, &MainWindow::imageUpdateSlotTwo);

	// right buttons, UA -> IP
	QObject::connect(mainWindow, &MainWindow::sendToTrainSignal, imageProcessing, &ImageProcessing::sendToTrainSlot);
	QObject::connect(mainWindow, &MainWindow::sendToTestSignal, imageProcessing, &ImageProcessing::sendToTestSlot);
	QObject::connect(mainWindow, &MainWindow::runTrainSignal, imageProcessing, &ImageProcessing::runTrainSlot);
	QObject::connect(mainWindow, &MainWindow::runTestSignal, imageProcessing, &ImageProcessing::runTestSlot);
	QObject::connect(mainWindow, &MainWindow::resetTrainSignal, imageProcessing, &ImageProcessing::resetTrainSlot);
	QObject::connect(mainWindow, &MainWindow::resetTestSignal, imageProcessing, &ImageProcessing::resetTestSlot);

	// right buttons, IP -> UA
	QObject::connect(imageProcessing, &ImageProcessing::sendToTrainReplySignal, mainWindow, &MainWindow::sendToTrainReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::sendToTestReplySignal,	mainWindow, &MainWindow::sendToTestReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::runTrainReplySignal,	mainWindow, &MainWindow::runTrainReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::runTestReplySignal,		mainWindow, &MainWindow::runTestReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::resetTrainReplySignal,	mainWindow, &MainWindow::resetTrainReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::resetTestReplySignal,	mainWindow, &MainWindow::resetTestReplySlot);

	// bottom buttons, UA -> IP/VAR
	QObject::connect(mainWindow, &MainWindow::configureSignal, imageProcessing, &ImageProcessing::configureSlot);  // sends 
	QObject::connect(mainWindow, &MainWindow::getImageSignal, imageProcessing, &ImageProcessing::getImageSlot);
	QObject::connect(mainWindow, &MainWindow::sendToVARSignal, validationAndResponse, &ValidationAndResponse::sendToVARSlot);
	QObject::connect(mainWindow, &MainWindow::getFromVARSignal, validationAndResponse, &ValidationAndResponse::getFromVARSlot);
	QObject::connect(mainWindow, &MainWindow::newGameSignal, validationAndResponse, &ValidationAndResponse::newGameSlot);
	QObject::connect(mainWindow, &MainWindow::exitSignal, cameraReaderOne, &CameraReader::stop);
	QObject::connect(mainWindow, &MainWindow::exitSignal, cameraReaderTwo, &CameraReader::stop);

	// bottom buttons, IP/VAR -> UA
	QObject::connect(imageProcessing, &ImageProcessing::configureReplySignal, mainWindow, &MainWindow::configureReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::getImageReplySignal, mainWindow, &MainWindow::getImageReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::sendToVARReplySignal, mainWindow, &MainWindow::sendToVARReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::getFromVARReplySignal, mainWindow, &MainWindow::getFromVARReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::newGameReplySignal, mainWindow, &MainWindow::newGameReplySlot);

	// start thread
	cameraReaderOne->moveToThread(cameraReaderOneThread);
	cameraReaderTwo->moveToThread(cameraReaderTwoThread);
	cropOne->moveToThread(cropOneThread);
	cropTwo->moveToThread(cropTwoThread);
	
	cameraReaderOneThread->start();
	cameraReaderTwoThread->start();
	cropOneThread->start();
	cropTwoThread->start();

	return userApplication->run();
}