#include "../../headers/crop/Crop.h"


#define BINARY_THRESHOLD		120
#define CLOSING_SIZE			6
#define HOUGH_RO_STEP_SIZE		1
#define HOUGH_THETA_STEP_SIZE	1
#define HOUGH_WINDOW_SIZE		3
#define HOUGH_NUMBER_OF_LINES	10

#define SHOW_IMAGES true
#define GENERATE_IMAGES false
#define CLASSIFY false


void Crop::crop(Mat_<Vec3b> imgOriginal)
{
	Mat_<Vec3b> imgResizedColor;
	resize(imgOriginal, imgResizedColor, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	Mat_<uchar> imgResizedGrayscale;
	cv::cvtColor(imgResizedColor, imgResizedGrayscale, cv::COLOR_BGR2GRAY);

	// filter to reduce noise
	Mat_<uchar> imgGauss = gaussianFilter(imgResizedGrayscale, 7);

	// get binary image from filtered image
	Mat_<uchar> imgBinary = grayscaleToBinary(imgGauss, BINARY_THRESHOLD);

	// perform closing on binary image to remove unnecessarry details
	uchar selPattern[CLOSING_SIZE * CLOSING_SIZE] = { 0 };
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
	std::vector<Point2i> intersections;
	Mat_<Vec3b> imgIntersections = imgResizedColor.clone();
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

			if (!isInside(imgIntersections, intersection.y, intersection.x))
			{
				std::cout << "Lines [" << lines[i].ro << "," << lines[i].theta;
				std::cout << "] and [" << lines[j].ro << "," << lines[j].theta;
				std::cout << "] intersect out of the image, in [" << intersection << "]." << std::endl;
				continue;
			}

			intersections.push_back(intersection);
			drawCrossColor(imgIntersections, intersection, 50, Vec3b(0, 0, 255));
			std::cout << "Lines [" << lines[i].ro << "," << lines[i].theta;
			std::cout << "] and [" << lines[j].ro << "," << lines[j].theta;
			std::cout << "] intersect in [" << intersection << "]." << std::endl;
		}
		//std::cout << lines[i].first << " " << lines[i].second << std::endl;
	}

	std::cout << "Intersections:" << std::endl;
	for (Point2i intersection : intersections)
	{
		std::cout << intersection << std::endl;
	}
	std::cout << std::endl;

	// get corners from intersections
	Point2i cornerChessboardTL = intersections[0];
	Point2i cornerChessboardTR = intersections[0];
	Point2i cornerChessboardBL = intersections[0];
	Point2i cornerChessboardBR = intersections[0];
	Point2i cornerImageTL = Point2i(0, 0);
	Point2i cornerImageTR = Point2i(imgResizedColor.cols, 0);
	Point2i cornerImageBL = Point2i(0, imgResizedColor.rows);
	Point2i cornerImageBR = Point2i(imgResizedColor.cols, imgResizedColor.rows);
	double minDistanceTL = euclideanDistance(cornerChessboardTL, cornerImageTL);
	double minDistanceTR = euclideanDistance(cornerChessboardTR, cornerImageTR);
	double minDistanceBL = euclideanDistance(cornerChessboardBL, cornerImageBL);
	double minDistanceBR = euclideanDistance(cornerChessboardBR, cornerImageBR);

	for (int i = 1; i < intersections.size(); i++)
	{
		Point2i point = intersections[i];
		if (euclideanDistance(point, cornerImageTL) < minDistanceTL)
		{
			cornerChessboardTL = point;
			minDistanceTL = euclideanDistance(point, cornerImageTL);
		}
		if (euclideanDistance(point, cornerImageTR) < minDistanceTR)
		{
			cornerChessboardTR = point;
			minDistanceTR = euclideanDistance(point, cornerImageTR);
		}
		if (euclideanDistance(point, cornerImageBL) < minDistanceBL)
		{
			cornerChessboardBL = point;
			minDistanceBL = euclideanDistance(point, cornerImageBL);
		}
		if (euclideanDistance(point, cornerImageBR) < minDistanceBR)
		{
			cornerChessboardBR = point;
			minDistanceBR = euclideanDistance(point, cornerImageBR);
		}
	}

	std::cout << "minX: " << cornerChessboardTL << std::endl;
	std::cout << "maxX: " << cornerChessboardTR << std::endl;
	std::cout << "minY: " << cornerChessboardBL << std::endl;
	std::cout << "maxY: " << cornerChessboardBR << std::endl;
	Mat_<Vec3b> imgCorners = imgResizedColor.clone();
	drawCrossColor(imgCorners, cornerChessboardTL, 50, Vec3b(0, 0, 255));
	drawCrossColor(imgCorners, cornerChessboardTR, 50, Vec3b(0, 0, 255));
	drawCrossColor(imgCorners, cornerChessboardBL, 50, Vec3b(0, 0, 255));
	drawCrossColor(imgCorners, cornerChessboardBR, 50, Vec3b(0, 0, 255));

	// orders important
	std::vector<Point2f> srcCorners;
	srcCorners.push_back(cornerChessboardTL);
	srcCorners.push_back(cornerChessboardTR);
	srcCorners.push_back(cornerChessboardBR);
	srcCorners.push_back(cornerChessboardBL);
	std::vector<Point2f> dstCorners;
	dstCorners.push_back(Point2d(0, 0));
	dstCorners.push_back(Point2d(500, 0));
	dstCorners.push_back(Point2d(500, 500));
	dstCorners.push_back(Point2d(0, 500));

	// perform rectification
	Mat M = getPerspectiveTransform(srcCorners, dstCorners);
	Mat_<Vec3b> imgWarped;
	warpPerspective(imgResizedColor, imgWarped, M, Size(500, 500));

	//Mat_<Vec3b> imgCell00 = extractCell(0, 0, imgWarped);
	Mat_<Vec3b> imgCell77 = extractCell(7, 7, imgWarped);
	Mat_<Vec3b> imgCell34 = extractCell(3, 4, imgWarped);

	if (GENERATE_IMAGES)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				Mat_<Vec3b> imgCell = extractCell(i, j, imgWarped);
				//sprintf(fileName, "Cells\\cell_%02d%02d%02d.jpeg", fileNo, i, j);
				//imwrite(fileName, imgCell);
			}
		}
	}

	if (SHOW_IMAGES)
	{
		// visualize each step
		imshow("imgGrayscale", imgResizedGrayscale);
		imshow("imgGauss", imgGauss);
		imshow("imgBinary", imgBinary);
		imshow("imgClosed", imgClosed);
		imshow("imgCanny", imgCanny);
		imshow("imgLines", imgLines);
		imshow("imgIntersections", imgIntersections);
		imshow("imgCorners", imgCorners);
		imshow("imgWarped", imgWarped);
		//imshow("imgROI", imgROI);
		//imshow("imgCell00", imgCell00);
		imshow("imgCell77", imgCell77);
		imshow("imgCell34", imgCell34);

		waitKey();
	}

}