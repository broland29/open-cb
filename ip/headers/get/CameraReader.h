#pragma once

#include <QThread>
#include <QImage>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "spdlog/spdlog.h"
#include "../crop/Crop.h"


class CameraReader : public QObject
{
	Q_OBJECT

public:
	CameraReader(int cameraNo, std::shared_ptr<QMutex> imshowMutex);
	void doWork();
	void stop();
	void deleteLater();

	void toggleConfigure();
	void toggleGetImage();

private:
	bool isRunning;
	int cameraNo;

	// parameters set to request "Configure", "Get Image"
	bool configure;
	bool getImage;
	QMutex parameterMutex;  // https://stackoverflow.com/questions/4897912/how-to-properly-interrupt-a-qthread-infinite-loop
	
	// imshow is not thread safe!
	std::shared_ptr<QMutex> imshowMutex;

signals:
	void imageUpdateSignal(QImage image);
	void configureSignal(QString path);  // delegated to Crop
	void getImageSignal(QString path);  // delegated to Crop
};