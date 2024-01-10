#include "stdafx.h"
#include "Util.h"
#include "Conversion.h"
#include "Morphological.h"
#include "Hough.h"
#include "Canny.h"
#include "Filter.h"

#define IMAGE_WIDTH				500
#define IMAGE_HEIGHT			500
#define BINARY_THRESHOLD		150
#define CLOSING_SIZE			6
#define HOUGH_RO_STEP_SIZE		1
#define HOUGH_THETA_STEP_SIZE	1
#define HOUGH_WINDOW_SIZE		3
#define HOUGH_NUMBER_OF_LINES	10

#define TEST_FILE_PATH "Images\\img002.jpg"



Mat_<Vec3b> extractCell(Mat_<Vec3b> img, int cellRow, int cellCol)
{
	assert(img.rows == img.cols);
	int unit = img.rows / 8;
	Rect ROI(cellRow * unit, cellCol * unit, unit, unit);
	std::cout << unit;
	//Rect ROI(50, 50, 100, 100);
	Mat_<Vec3b> cell = img(ROI);
	return cell;
}

int main()
{
	int fileNo = 1;
	char fileName[100];
	while (true)
	{
		sprintf(fileName, "Images\\img_%02d.jpeg", fileNo++);

		Mat_<uchar> imgOriginalGrayscale = imread(fileName, IMREAD_GRAYSCALE);
		if (imgOriginalGrayscale.empty())
		{
			std::cout << "Break: file " << fileName << " not found!" << std::endl;
			break;  // no more images
		}
		Mat_<Vec3b> imgOriginalColor = imread(fileName, IMREAD_COLOR);
		if (imgOriginalColor.empty())
		{
			std::cout << "Break: file " << fileName << " not found!" << std::endl;
			break;
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
		uchar selPattern[CLOSING_SIZE * CLOSING_SIZE ] = { 0 };
		const cv::Mat_<uchar> sel = cv::Mat(7, 7, CV_8UC1, selPattern);
		Mat_<uchar> imgClosed = closing(imgBinary, sel);

		// perform canny edge detection on closed image
		Mat_<uchar> imgCanny = canny(imgClosed);

		// perform hough on the image resulted from canny
		std::vector<Point2i> points = getPointsFromBinary(imgCanny);
		std::vector<lineRoTheta> lines = hough(
			points,
			imgResizedColor,
			HOUGH_RO_STEP_SIZE,
			HOUGH_THETA_STEP_SIZE,
			HOUGH_WINDOW_SIZE,
			HOUGH_NUMBER_OF_LINES
		);

		// draw the resulting lines from Hough
		Mat_<Vec3b> imgLines = imgResizedColor.clone();
		for (lineRoTheta line : lines)
		{
			imgLines = line.drawLine(imgLines);
		}

		// get intersection of each line with each other, and if inside image, draw a cross
		Mat_<Vec3b> imgCorners = imgResizedColor.clone();
		for (int i = 0; i < lines.size(); i++)
		{
			for (int j = i + 1; j < lines.size(); j++)
			{
				Point2i intersection;
				int ret = lines[i].getIntersection(lines[j], intersection);
				if (ret == 1)
				{
					std::cout << "Lines [" << lines[i].ro << "," << lines[i].theta;
					std::cout << "] and [" << lines[j].ro << "," << lines[j].theta;
					std::cout << "] are parallel!" << std::endl;
					continue;
				}

				if (!isInside(imgCorners, intersection.y, intersection.x))
				{
					std::cout << "Lines [" << lines[i].ro << "," << lines[i].theta;
					std::cout << "] and [" << lines[j].ro << "," << lines[j].theta;
					std::cout << "] intersect out of the image, in [" << intersection << "]." << std::endl;
					continue;
				}

				drawCrossColor(imgCorners, intersection, 50, Vec3b(0, 0, 255));
				std::cout << "Lines [" << lines[i].ro << "," << lines[i].theta;
				std::cout << "] and [" << lines[j].ro << "," << lines[j].theta;
				std::cout << "] intersect in [" << intersection << "]." << std::endl;
			}
			//std::cout << lines[i].first << " " << lines[i].second << std::endl;
		}

		/*
		Mat_<uchar> imgCorners;
		imgBinary.copyTo(imgCorners);
		for (int i = 0; i < lines.size(); i++)
		{
			for (int j = i + 1; j < lines.size(); j++)
			{
				Point2i intersection;
				if (getIntersectionOfLines(lines[i], lines[j], imgCorners, intersection) == 0)
				{
					drawCrossGrayscale(imgCorners, intersection, 50, 100);
					std::cout << "Lines [" << lines[i].first << "," << lines[i].second;
					std::cout << "] and [" << lines[j].first << "," << lines[j].second;
					std::cout << "] intersect in [" << intersection << "]." << std::endl;
				}
			}
			//std::cout << lines[i].first << " " << lines[i].second << std::endl;
		}
		*/

		// visualize each step
		imshow("imgGrayscale", imgResizedGrayscale);
		imshow("imgGauss", imgGauss);
		imshow("imgBinary", imgBinary);
		imshow("imgClosed", imgClosed);
		imshow("imgCanny", imgCanny);
		imshow("imgLines", imgLines);
		imshow("imgCorners1", imgCorners);
		waitKey();
	}

	/*
	Point2d ul(135, 93);
	Point2d ur(380, 90);
	Point2d dl(63, 410);
	Point2d dr(470, 400);
	drawCrossGrayscale(imgBinary, ul, 50, 100);
	drawCrossGrayscale(imgBinary, ur, 50, 100);
	drawCrossGrayscale(imgBinary, dl, 50, 100);
	drawCrossGrayscale(imgBinary, dr, 50, 100);

	std::vector<Point2f> srcCorners;
	srcCorners.push_back(ul);
	srcCorners.push_back(ur);
	srcCorners.push_back(dr);
	srcCorners.push_back(dl);

	std::vector<Point2f> dstCorners;
	dstCorners.push_back(Point2d(0, 0));
	dstCorners.push_back(Point2d(500, 0));
	dstCorners.push_back(Point2d(500, 500));
	dstCorners.push_back(Point2d(0, 500));

	Mat M = getPerspectiveTransform(srcCorners, dstCorners);
	Mat imgWarped;
	warpPerspective(imgResizedColor, imgWarped, M, Size(500, 500));

	imshow("imgCorners", imgBinary);
	imshow("imgWarped", imgWarped);

	int border = 50;
	Rect ROI(border, border, 500 - 2 * border, 500 - 2 * border);
	Mat_<Vec3b> imgROI = imgWarped(ROI);
	Mat_<Vec3b> imgCell = extractCell(imgROI, 7, 7);
	imshow("imgROI", imgROI);
	imshow("imgCell", imgCell);

	//Mat_<Vec3b> cell()


	waitKey();
	*/
	return 0;
}