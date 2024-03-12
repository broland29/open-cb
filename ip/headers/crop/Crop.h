#pragma once

#include "../Util.h"
#include "Filter.h"
#include "Conversion.h"
#include "Morphological.h"
#include "Canny.h"
#include "Hough.h"
#include <QObject>
#include <QMutex>

class Crop : public QObject
{
	Q_OBJECT

public:
	Crop(std::shared_ptr<QMutex> imshowMutex);

private:
	// corners of chessboard in order: TL, TR, BR, BL
	bool configured;
	std::vector<Point2f> corners;

	// imshow is not thread safe!
	std::shared_ptr<QMutex> imshowMutex;

public slots:
	void configureSlot(QString path);
	void getImageSlot(QString path);

signals:
	void configureReplySignal(bool succeeded);
	void getImageReplySignal(QString board);
};