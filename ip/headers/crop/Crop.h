#pragma once

#include <QObject>
#include <QMutex>
#include <spdlog/spdlog.h>

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

namespace Crop
{
	// gets the corners of the chessboard
	int configure(
		Mat_<Vec3b> imgOriginal,				// image from which the corner positions will be extracted
		std::vector<Point2f> corners,			// will be filled (success case)
		bool& configured,						// will be set (success case)
		std::shared_ptr<QMutex> imshowMutex		// since imshow not thread-safe
	);

	// does experimental processing: gets warped image, image without border and "corner cells of interest"
	int getImage(
		Mat_<Vec3b> imgOriginal,				// image which will be processed
		std::vector<Point2f> corners,			// corners of the chessboard, previously filled
		bool configured,						// should be set (meaning corners were filled)
		std::shared_ptr<QMutex> imshowMutex		// since imshow not thread-safe
	);
};