#include "ip/headers/ImageProcessing.h"
#include "log/headers/Logger.h"
#include "ua/headers/UserApplication.h"
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
	CameraHandler* cameraHandlerLeft = imageProcessing->cameraHandlerLeft;
	CameraHandler* cameraHandlerRight = imageProcessing->cameraHandlerRight;
	MainWindow* mainWindow = userApplication->mainWindow;
	QApplication* application = userApplication->application;

	// create worker threads
	QThread* imageProcessingThread = new QThread;

	QThread* cameraHandlerLeftThread = new QThread;
	QObject::connect(cameraHandlerLeftThread, &QThread::started, cameraHandlerLeft, &CameraHandler::doWork);
	QObject::connect(cameraHandlerLeft, &CameraHandler::stop, cameraHandlerLeftThread, &QThread::quit);
	QObject::connect(cameraHandlerLeftThread, &QThread::finished, cameraHandlerLeft, &CameraHandler::deleteLater);

	QThread* cameraHandlerRightThread = new QThread;
	QObject::connect(cameraHandlerRightThread, &QThread::started, cameraHandlerRight, &CameraHandler::doWork);
	QObject::connect(cameraHandlerRight, &CameraHandler::stop, cameraHandlerRightThread, &QThread::quit);
	QObject::connect(cameraHandlerRightThread, &QThread::finished, cameraHandlerRight, &CameraHandler::deleteLater);

	// cross - thread communication
	QObject::connect(cameraHandlerLeft, &CameraHandler::previewImageReadySignal, mainWindow, &MainWindow::previewImageReadySlotLeft);
	QObject::connect(cameraHandlerRight, &CameraHandler::previewImageReadySignal, mainWindow, &MainWindow::previewImageReadySlotRight);

	QObject::connect(mainWindow, &MainWindow::validateMoveSignal, validationAndResponse, &ValidationAndResponse::validateMoveSlot);
	QObject::connect(mainWindow, &MainWindow::discardMoveSignal, validationAndResponse, &ValidationAndResponse::discardMoveSlot);
	QObject::connect(mainWindow, &MainWindow::newGameSignal, validationAndResponse, &ValidationAndResponse::newGameSlot);

	QObject::connect(validationAndResponse, &ValidationAndResponse::validateMoveReplySignal, mainWindow, &MainWindow::validateMoveReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::discardMoveReplySignal, mainWindow, &MainWindow::discardMoveReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::newGameReplySignal, mainWindow, &MainWindow::newGameReplySlot);

	QObject::connect(mainWindow, &MainWindow::changeClassifierSignal, imageProcessing, &ImageProcessing::changeClassifierSlot);
	QObject::connect(mainWindow, &MainWindow::saveClassifierSignal, imageProcessing, &ImageProcessing::saveClassifierSlot);
	QObject::connect(mainWindow, &MainWindow::loadClassifierSignal, imageProcessing, &ImageProcessing::loadClassifierSlot);
	QObject::connect(mainWindow, &MainWindow::trainClassifierSignal, imageProcessing, &ImageProcessing::trainClassifierSlot);
	QObject::connect(mainWindow, &MainWindow::testClassifierSignal, imageProcessing, &ImageProcessing::testClassifierSlot);
	QObject::connect(mainWindow, &MainWindow::classifyBoardSignal, imageProcessing, &ImageProcessing::classifyBoardSlot);

	QObject::connect(mainWindow, &MainWindow::testConfigureSignal, imageProcessing, &ImageProcessing::testConfigureSlot);
	QObject::connect(mainWindow, &MainWindow::configureSignal, imageProcessing, &ImageProcessing::configureSlot);
	QObject::connect(mainWindow, &MainWindow::testCropAndLabelSignal, imageProcessing, &ImageProcessing::testCropAndLabelSlot);
	QObject::connect(mainWindow, &MainWindow::cropAndLabelSignal, imageProcessing, &ImageProcessing::cropAndLabelSlot);
	QObject::connect(mainWindow, &MainWindow::shuffleAndSplitSignal, imageProcessing, &ImageProcessing::shuffleAndSplitSlot);
	QObject::connect(mainWindow, &MainWindow::clearAllImagesSignal, imageProcessing, &ImageProcessing::clearAllImagesSlot);
	
	QObject::connect(mainWindow, &MainWindow::parametersChangedSignal, imageProcessing, &ImageProcessing::parametersChangedSlot);


	QObject::connect(imageProcessing, &ImageProcessing::changeClassifierReplySignal, mainWindow, &MainWindow::changeClassifierReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::saveClassifierReplySignal, mainWindow, &MainWindow::saveClassifierReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::loadClassifierReplySignal, mainWindow, &MainWindow::loadClassifierReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::trainClassifierReplySignal, mainWindow, &MainWindow::trainClassifierReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::testClassifierReplySignal, mainWindow, &MainWindow::testClassifierReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::classifyBoardReplySignal, mainWindow, &MainWindow::classifyBoardReplySlot);
	
	QObject::connect(imageProcessing, &ImageProcessing::testConfigureReplySignal, mainWindow, &MainWindow::testConfigureReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::configureReplySignal, mainWindow, &MainWindow::configureReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::testCropAndLabelReplySignal, mainWindow, &MainWindow::testCropAndLabelReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::cropAndLabelReplySignal, mainWindow, &MainWindow::cropAndLabelReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::shuffleAndSplitReplySignal, mainWindow, &MainWindow::shuffleAndSplitReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::clearAllImagesReplySignal, mainWindow, &MainWindow::clearAllImagesReplySlot);
	
	QObject::connect(imageProcessing, &ImageProcessing::parametersChangedReplySignal, mainWindow, &MainWindow::parametersChangedReplySlot);


	QObject::connect(application, &QApplication::aboutToQuit, imageProcessing, &ImageProcessing::beforeQuit, Qt::DirectConnection);  // with default Qt::QueuedConnection exits before done

	// start thread
	imageProcessing->moveToThread(imageProcessingThread);

	cameraHandlerLeft->moveToThread(cameraHandlerLeftThread);
	cameraHandlerRight->moveToThread(cameraHandlerRightThread);
	
	imageProcessingThread->start();
	cameraHandlerLeftThread->start();
	cameraHandlerRightThread->start();

	return userApplication->run();
}