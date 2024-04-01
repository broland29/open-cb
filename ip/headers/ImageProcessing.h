#pragma once

#include "camera_handling/CameraHandler.h"
#include "camera_handling/SignalWaiter.h"
#include "classification/Classifier.h"
#include "configuration/Configurer.h"
#include "file_handling/FileHandler.h"

#include "Common.h"
#include "EncodingMapperIP.h"

#include <opencv2/core/utils/logger.hpp>

#include <QTimer>
#include <QEventLoop>
#include <QObject>
#include <QVariant>

#define DEFAULT_LEFT_CAMERA_INDEX 0
#define DEFAULT_RIGHT_CAMERA_INDEX 1


class ImageProcessing : public QObject
{
	Q_OBJECT

public:
	int leftCameraIndex = DEFAULT_LEFT_CAMERA_INDEX;
	int rightCameraIndex = DEFAULT_RIGHT_CAMERA_INDEX;
	CameraHandler* cameraHandlerLeft;
	CameraHandler* cameraHandlerRight;

private:
	Classifier* classifier;
	Configurer* configurerLeft;
	Configurer* configurerRight;
	FileHandler* fileHandler;


	// common fields
	std::shared_ptr<QMutex> imshowMutex;
	unsigned int count;  // the count of images which were saved; common since success case only when both cameras save

public:
	ImageProcessing();
	static void test();

private:
	int getImage(SignalWaiter* signalWaiter, Mat_<Vec3b>& img);
	void configure(bool isTest);
	void cropAndLabel(std::string board[64], bool isTest);

signals:
	void changeClassifierReplySignal(bool succeeded, QString message);
	void saveClassifierReplySignal(bool succeeded, QString message);
	void loadClassifierReplySignal(bool succeeded, QString message);
	void trainClassifierReplySignal(bool succeeded, QString message);
	void testClassifierReplySignal(bool succeeded, QString message);
	void classifyBoardReplySignal(bool succeeded, QString message);

	void testConfigureReplySignal(bool succeeded, QString message);
	void configureReplySignal(bool succeeded, QString message);
	void testCropAndLabelReplySignal(bool succeeded, QString message);
	void cropAndLabelReplySignal(bool succeeded, QString message);
	void shuffleAndSplitReplySignal(bool succeeded, QString message);
	void clearAllImagesReplySignal(bool succeeded, QString message);
	void changeSettingsReplySignal(bool succeeded, QString message);

public slots:
	void changeClassifierSlot(QString newClassifierName);
	void saveClassifierSlot(QString folderPath);
	void loadClassifierSlot(QString folderPath);
	void trainClassifierSlot();
	void testClassifierSlot();
	void classifyBoardSlot();

	void testConfigureSlot();
	void configureSlot();
	void testCropAndLabelSlot(QString board);
	void cropAndLabelSlot(QString board);
	void shuffleAndSplitSlot();
	void clearAllImagesSlot();
	void changeSettingsSlot();
};