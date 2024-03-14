#pragma once

#include <QThread>
#include <QImage>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "spdlog/spdlog.h"
#include "../crop/Crop.h"
#include <QObject>
#include <QVariant>


class CameraReader : public QObject
{
	Q_OBJECT

public:

private:
	bool isRunning;
	int cameraNo;

	// request parameters set to request "Configure", "Get Image"
	bool isImageRequested;
	std::string requestImagePathPrefix;
	QMutex requestMutex;  // https://stackoverflow.com/questions/4897912/how-to-properly-interrupt-a-qthread-infinite-loop

	// imshow is not thread safe!
	std::shared_ptr<QMutex> imshowMutex;


public:
	CameraReader(int cameraNo, std::shared_ptr<QMutex> imshowMutex);
	void doWork();
	void stop();
	void deleteLater();

	void toggleRequest(std::string requestImagePathPrefix);

signals:
	void imageUpdateSignal(QImage image);
	void imageSaved(QString path);
};