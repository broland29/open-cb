#pragma once

#include "../com/headers/ParametersHandler.h"
#include "camera_handling/CameraHandler.h"
#include "camera_handling/SignalWaiter.h"
#include "classification/Classifier.h"
#include "configuration/Configurer.h"
#include "file_handling/FileHandler.h"

#include "CommonIP.h"

#include <opencv2/core/utils/logger.hpp>

#include <QTimer>
#include <QEventLoop>
#include <QObject>
#include <QVariant>
#include <QVector>
#include <QString>




class ImageProcessing : public QObject
{
	Q_OBJECT

public:
	QThread* cameraHandlerLeftThread = NULL;
	QThread* cameraHandlerRightThread = NULL;
	CameraHandler* cameraHandlerLeft;
	CameraHandler* cameraHandlerRight;

private:
	Parameters parameters;
	Classifier* classifier;
	Configurer* configurerLeft;
	Configurer* configurerRight;


	// common fields
	std::shared_ptr<QMutex> imshowMutex;

public:
	ImageProcessing();
	static void test();

private:
	int getImage(SignalWaiter* signalWaiter, Mat_<Vec3b>& img);
	void configure(bool isTest);
	void cropAndLabel(QVector<QString> encodings, bool isTest);
	void setupCameraHandlerLeft();
	void setupCameraHandlerRight();

signals:
	void changeClassifierReplySignal(bool succeeded, QString message);
	void saveClassifierReplySignal(bool succeeded, QString message);
	void loadClassifierReplySignal(bool succeeded, QString message);
	void trainClassifierReplySignal(bool succeeded, QString message);
	void testClassifierReplySignal(bool succeeded, QString message);
	void classifyBoardReplySignal(bool succeeded, QString message, QVector<QString> encodings);

	void testConfigureReplySignal(bool succeeded, QString message);
	void configureReplySignal(bool succeeded, QString message);
	void testCropAndLabelReplySignal(bool succeeded, QString message);
	void cropAndLabelReplySignal(bool succeeded, QString message);
	void shuffleAndSplitReplySignal(bool succeeded, QString message);
	void clearAllImagesReplySignal(bool succeeded, QString message);
	void changeSettingsReplySignal(bool succeeded, QString message);

	void setParametersReplySignal(bool succeeded, QString message);
	void getParametersReplySignal(Parameters parameters);

	void previewImageReadyLeftSignal(QImage image);
	void previewImageReadyRightSignal(QImage image);

public slots:
	void changeClassifierSlot(QString newClassifierName);
	void saveClassifierSlot();
	void loadClassifierSlot();
	void trainClassifierSlot();
	void testClassifierSlot();
	void classifyBoardSlot();

	void testConfigureSlot();
	void configureSlot();
	void testCropAndLabelSlot(QVector<QString> encodings);
	void cropAndLabelSlot(QVector<QString> encodings);
	void shuffleAndSplitSlot();
	void clearAllImagesSlot();
	void changeSettingsSlot();

	void setParametersSlot(Parameters parameters);
	void getParametersSlot();

	void previewImageReadyLeftSlot(QImage image);
	void previewImageReadyRightSlot(QImage image);

	void beforeQuit();
};
