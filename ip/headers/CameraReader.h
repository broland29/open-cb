#pragma once

#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "spdlog/spdlog.h"


class CameraReader : public QObject
{
	Q_OBJECT

public:
	CameraReader(int cameraNo);
	void doWork();
	void stop();

private:
	bool isRunning;
	int cameraNo;

signals:
	void imageUpdateSignal(QImage image);
};