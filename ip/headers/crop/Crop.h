#pragma once

#include <QObject>
#include <QMutex>
#include <spdlog/spdlog.h>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

#include "../Util.h"
#include "Filter.h"
#include "Conversion.h"
#include "Morphological.h"
#include "Canny.h"
#include "Hough.h"

#define BINARY_THRESHOLD		120
#define CLOSING_SIZE			6
#define HOUGH_RO_STEP_SIZE		1
#define HOUGH_THETA_STEP_SIZE	1
#define HOUGH_WINDOW_SIZE		3
#define HOUGH_NUMBER_OF_LINES	20

#define SHOW_IMAGES				true
#define CONCAT_IMAGES			true
#define GENERATE_IMAGES			false
#define CLASSIFY				false




enum class CameraSide
{
	LEFT,
	RIGHT
};


class Crop
{
public:
private:
public:
	static int sendToTrain(
		Mat_<Vec3b> img,
		std::vector<Point2f> corners,			
		bool configured,						
		CameraSide cameraSide,
		std::string board[8][8],
		int imageCount
	);

	static int sendToTest(
		Mat_<Vec3b> img,
		std::vector<Point2f> corners,
		bool configured,
		CameraSide cameraSide,
		std::string board[8][8],
		int imageCount
	);

	static int resetTrain();

	static int resetTest();

	// gets the corners of the chessboard
	static int configure(
		Mat_<Vec3b> imgOriginal,				// image from which the corner positions will be extracted
		std::vector<Point2f>& corners,			// will be filled (success case)
		bool& configured,						// will be set (success case)
		std::shared_ptr<QMutex> imshowMutex		// since imshow not thread-safe
	);

	// does experimental processing: gets warped image, image without border and "corner cells of interest"
	static int getImage(
		Mat_<Vec3b> imgOriginal,				// image which will be processed
		std::vector<Point2f> corners,			// corners of the chessboard, previously filled
		bool configured,						// should be set (meaning corners were filled)
		std::shared_ptr<QMutex> imshowMutex		// since imshow not thread-safe
	);

private:
	// coordinate system transformation: left -> main (see cams.drawio)
	static void leftToMain(int lr, int lc, int& mr, int& mc);

	// coordinate system transformation: right -> main (see cams.drawio)
	static void rightToMain(int rr, int rc, int& mr, int& mc);

	static int warpAndRemoveBorder(
		Mat_<Vec3b> imgOriginal,
		std::vector<Point2f> corners,
		bool configured,
		Mat_<Vec3b>& imgWarped,
		Mat_<Vec3b>& imgNoBorder
	);

	static int sendToFolder(
		Mat_<Vec3b> img,				// the image
		std::vector<Point2f> corners,
		bool configured,
		CameraSide cameraSide,			// self explanatory
		std::string board[8][8],		// encoding of chess pieces, following indexation convention, as user gives through GUI = subfolder
		int imageCount,					// the number associated with the image
		std::string basePath			// train or test folder path
	);
	
	static int resetFolder(std::string basePath);
};