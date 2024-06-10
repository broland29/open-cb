#pragma once

#include "../CommonIP.h"
#include "BFS.h"
#include "Canny.h"
#include "Conversion.h"
#include "Filter.h"
#include "Hough.h"
#include "Morphological.h"
#include "../file_handling/FileHandler.h"

#include <QObject>
#include <QMutex>


#define DEFAULT_BINARY_THRESHOLD		120

#define DEFAULT_CLOSING_SIZE		6

#define DEFAULT_HOUGH_RO_STEP_SIZE		1
#define DEFAULT_HOUGH_THETA_STEP_SIZE	1
#define DEFAULT_HOUGH_WINDOW_SIZE		3
#define DEFAULT_HOUGH_NUMBER_OF_LINES	20


enum class CameraSide
{
	LEFT,
	RIGHT
};


class Configurer
{
public:
	uchar binaryThreshold = DEFAULT_BINARY_THRESHOLD;
	int closingSize = DEFAULT_CLOSING_SIZE;
	
	int houghRoStepSize = DEFAULT_HOUGH_RO_STEP_SIZE;
	int houghThetaStepSize = DEFAULT_HOUGH_THETA_STEP_SIZE;
	int houghWindowSize = DEFAULT_HOUGH_WINDOW_SIZE;
	int houghNumberOfLines = DEFAULT_HOUGH_NUMBER_OF_LINES;

private:
	
	CameraSide cameraSide;
	std::vector<Point2f> corners;			// corners of chessboard
	bool configured;						// true if corners set at least once
	std::shared_ptr<QMutex> imshowMutex;

public:
	Configurer(CameraSide cameraSide, std::shared_ptr<QMutex> imshowMutex);

	// gets the corners of the chessboard
	int configure(
		Mat_<Vec3b> img,  // image from which the corner positions will be extracted
		bool isTest,
		bool showImages,
		bool concatImages
	);

	int cropAndLabel(
		Mat_<Vec3b> img,
		QVector<QString> encodings,  // labelFolder for each cell
		bool isTest,
		bool concatImages,
		int borderTop, 
		int borderRight, 
		int borderBottom, 
		int borderLeft
	);

	// save cell images on disk. while this is an overhead, classifiers may need different
	//     image formats (ex: CNN), so cannot universally just pass an array of Mats
	int prepareCellImages(
		Mat_<Vec3b> imgOriginal,
		int borderTop,
		int borderRight,
		int borderBottom,
		int borderLeft
	);

private:
	// actually extracts two cells
	Mat_<Vec3b> extractCell(int i, int j, Mat_<Vec3b> img, int borderTop, int borderRight, int borderBottom, int borderLeft);

	// coordinate system transformation: left -> main (see cams.drawio)
	void leftToMain(int lr, int lc, int& mr, int& mc);

	// coordinate system transformation: right -> main (see cams.drawio)
	void rightToMain(int rr, int rc, int& mr, int& mc);

	int warpAndRemoveBorder(
		Mat_<Vec3b> imgOriginal,
		Mat_<Vec3b>& imgWarped,
		Mat_<Vec3b>& imgNoBorder,
		int borderTop,
		int borderRight,
		int borderBottom,
		int borderLeft
	);

	// Draws a cross on color image img, "around" point p, with given diameter
// and optionally color (white default)
	void drawCrossColor(
		Mat_<Vec3b> img,
		Point2i p,
		int diameter,
		Vec3b color = Vec3b(255, 255, 255)
	);

	// Extract object pixels in a list of points
	std::vector<Point2i> getPointsFromBinary(
		Mat_<uchar> img,
		int objectPixelColor = 255
	);

	double euclideanDistance(Point2i a, Point2i b);

};

