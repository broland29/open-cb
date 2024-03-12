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
	void setSaveAFrame(bool value);
	bool getSaveAFrame();

private:
	bool isRunning;
	int cameraNo;
	bool saveAFrame;
	QMutex saveAFrameMutex;  // https://stackoverflow.com/questions/4897912/how-to-properly-interrupt-a-qthread-infinite-loop
	std::shared_ptr<QMutex> imshowMutex;  // imshow is not thread safe!

signals:
	void imageUpdateSignal(QImage image);
};