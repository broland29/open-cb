#include "ip/headers/ImageProcessing.h"
#include "com/headers/Logger.h"
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

	MainWindow* mainWindow = userApplication->mainWindow;
	QApplication* application = userApplication->application;

	// create worker threads
	QThread* imageProcessingThread = new QThread;

	// cross - thread communication
	QObject::connect(imageProcessing, &ImageProcessing::previewImageReadyLeftSignal, mainWindow, &MainWindow::previewImageReadySlotLeft);
	QObject::connect(imageProcessing, &ImageProcessing::previewImageReadyRightSignal, mainWindow, &MainWindow::previewImageReadySlotRight);

	QObject::connect(mainWindow, &MainWindow::validateMoveSignal, validationAndResponse, &ValidationAndResponse::validateMoveSlot);
	QObject::connect(mainWindow, &MainWindow::discardMoveSignal, validationAndResponse, &ValidationAndResponse::discardMoveSlot);
	QObject::connect(mainWindow, &MainWindow::openLogsSignal, validationAndResponse, &ValidationAndResponse::openLogsSlot);
	QObject::connect(mainWindow, &MainWindow::newGameSignal, validationAndResponse, &ValidationAndResponse::newGameSlot);
	QObject::connect(mainWindow, &MainWindow::surrenderSignal, validationAndResponse, &ValidationAndResponse::surrenderSlot);
	QObject::connect(mainWindow, &MainWindow::offerDrawSignal, validationAndResponse, &ValidationAndResponse::offerDrawSlot);

	QObject::connect(validationAndResponse, &ValidationAndResponse::validateMoveReplySignal, mainWindow, &MainWindow::validateMoveReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::discardMoveReplySignal, mainWindow, &MainWindow::discardMoveReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::openLogsReplySignal, mainWindow, &MainWindow::openLogsReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::newGameReplySignal, mainWindow, &MainWindow::newGameReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::surrenderReplySignal, mainWindow, &MainWindow::surrenderReplySlot);
	QObject::connect(validationAndResponse, &ValidationAndResponse::offerDrawReplySignal, mainWindow, &MainWindow::offerDrawReplySlot);

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
	
	QObject::connect(mainWindow, &MainWindow::setParametersSignal, imageProcessing, &ImageProcessing::setParametersSlot);
	QObject::connect(mainWindow, &MainWindow::getParametersSignal, imageProcessing, &ImageProcessing::getParametersSlot);


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
	
	QObject::connect(imageProcessing, &ImageProcessing::setParametersReplySignal, mainWindow, &MainWindow::setParametersReplySlot);
	QObject::connect(imageProcessing, &ImageProcessing::getParametersReplySignal, mainWindow, &MainWindow::getParametersReplySlot);


	QObject::connect(application, &QApplication::aboutToQuit, imageProcessing, &ImageProcessing::beforeQuit, Qt::DirectConnection);  // with default Qt::QueuedConnection exits before done

	// start thread
	imageProcessing->moveToThread(imageProcessingThread);
	
	imageProcessingThread->start();

	return userApplication->run();
}