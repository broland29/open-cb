#include "stdafx.h"
#include "Util.h"
#include "Conversion.h"
#include "Morphological.h"
#include "Hough.h"
#include "Canny.h"
#include "Filter.h"

#define BINARY_THRESHOLD 100
#define IMAGE_WIDTH 500
#define IMAGE_HEIGHT 500
#define HOUGH_RO_STEP_SIZE 10
#define HOUGH_THETA_STEP_SIZE 10
#define HOUGH_WINDOW_SIZE 5

#define TEST_FILE_PATH "Images\\img001.jpg"


int main()
{
	// read image in both grayscale and color version
	Mat_<uchar> imgOriginalGrayscale = imread(TEST_FILE_PATH, IMREAD_GRAYSCALE);
	if (imgOriginalGrayscale.empty())
	{
		std::cout << "Could not open image!" << std::endl;
		return 1;
	}
	Mat_<Vec3b> imgOriginalColor = imread(TEST_FILE_PATH, IMREAD_COLOR);
	if (imgOriginalColor.empty())
	{
		std::cout << "Could not open image!" << std::endl;
		return 1;
	}

	// resize both to same dimensions
	Mat_<uchar> imgResizedGrayscale;
	resize(imgOriginalGrayscale, imgResizedGrayscale, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));
	Mat_<Vec3b> imgResizedColor;
	resize(imgOriginalColor, imgResizedColor, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	// filter to reduce noise
	Mat_<uchar> imgGauss = gaussianFilter(imgResizedGrayscale, 7);

	// get binary image from filtered image
	Mat_<uchar> imgBinary = grayscaleToBinary(imgGauss, BINARY_THRESHOLD);

	// perform closing on binary image to remove unnecessarry details
	uchar selPattern[] = {
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0
	};
	const cv::Mat_<uchar> sel = cv::Mat(7, 7, CV_8UC1, selPattern);
	Mat_<uchar> imgClosed = closing(imgBinary, sel);

	// perform canny edge detection on closed image
	Mat_<uchar> imgCanny = canny(imgClosed);

	// perform hough on the image resulted from canny
	std::vector<Point2i> points = getPointsFromBinary(imgCanny);
	Mat_<Vec3b> imgHough = hough(points, imgResizedColor);

	// visualize each step
	imshow("imgGrayscale", imgResizedGrayscale);
	imshow("imgGauss",	imgGauss);
	imshow("imgBinary", imgBinary);
	imshow("imgClosed", imgClosed);
	imshow("imgCanny", imgCanny);
	imshow("imgHough", imgHough);
	waitKey();

	return 0;
}