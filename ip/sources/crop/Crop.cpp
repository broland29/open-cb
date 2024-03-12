#include "../../headers/crop/Crop.h"


#define BINARY_THRESHOLD		120
#define CLOSING_SIZE			6
#define HOUGH_RO_STEP_SIZE		1
#define HOUGH_THETA_STEP_SIZE	1
#define HOUGH_WINDOW_SIZE		3
#define HOUGH_NUMBER_OF_LINES	20

#define SHOW_IMAGES true
#define CONCAT_IMAGES true
#define GENERATE_IMAGES false
#define CLASSIFY false




Crop::Crop(std::shared_ptr<QMutex> imshowMutex)
{
	configured = false;
	this->imshowMutex = imshowMutex;
}


void Crop::configureSlot(QString path)
{
	Mat_<Vec3b> imgOriginal = imread(path.toStdString(), IMREAD_COLOR);

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
	corners.clear();
	corners.push_back(cornerChessboardTL);
	corners.push_back(cornerChessboardTR);
	corners.push_back(cornerChessboardBR);
	corners.push_back(cornerChessboardBL);

	this->configured = true;

	if (SHOW_IMAGES)
	{
		imshowMutex->lock();
		if (CONCAT_IMAGES)
		{
			Mat_<uchar> grayscaleImages;
			Mat_<Vec3b> colorImages;

			Mat_<uchar> grayscaleFiller = Mat_<uchar>::zeros(IMAGE_HEIGHT, IMAGE_WIDTH);
			Mat_<uchar> grayscaleRowOne;
			Mat_<uchar> grayscaleRowTwo;
			
			cv::hconcat(imgResizedGrayscale, imgGauss, grayscaleRowOne);
			cv::hconcat(grayscaleRowOne, imgBinary, grayscaleRowOne);
			cv::hconcat(imgClosed, imgCanny, grayscaleRowTwo);
			cv::hconcat(grayscaleRowTwo, grayscaleFiller, grayscaleRowTwo);
			//cv::vconcat(grayscaleRowOne, grayscaleRowTwo, grayscaleImages);

			cv::hconcat(imgLines, imgIntersections, colorImages);
			cv::hconcat(colorImages, imgCorners, colorImages);

			//imshow("grayscaleImages", grayscaleImages);
			imshow("grayscaleRowOne", grayscaleRowOne);
			imshow("grayscaleRowTwo", grayscaleRowTwo);
			imshow("colorImages", colorImages);
			moveWindow("grayscaleRowOne", 50, 50);
			moveWindow("grayscaleRowTwo", 50, 50);
			moveWindow("colorImages", 50, 50);
		}
		else
		{
			imshow("imgGrayscale", imgResizedGrayscale);		// GS
			imshow("imgGauss", imgGauss);						// GS
			imshow("imgBinary", imgBinary);						// GS
			imshow("imgClosed", imgClosed);					    // GS
			imshow("imgCanny", imgCanny);						// GS
			imshow("imgLines", imgLines);						// Color
			imshow("imgIntersections", imgIntersections);		// Color
			imshow("imgCorners", imgCorners);					// Color
		}
		waitKey();
		imshowMutex->unlock();
	}
}

void Crop::getImageSlot(QString path)
{
	if (!configured)
	{
		std::cout << "Not configured, returning" << std::endl;
		return;  // todo - error messages
	}

	std::string path_ = path.toStdString();
	Mat_<Vec3b> imgOriginal = imread(path_, IMREAD_COLOR);
	if (imgOriginal.empty())
	{
		std::cout << "Could not open " << path_ << std::endl;
		return;  // todo - error messages
	}

	Mat_<Vec3b> imgResizedColor;
	resize(imgOriginal, imgResizedColor, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	std::vector<Point2f> dstCorners;
	dstCorners.push_back(Point2d(0, 0));						// TL 
	dstCorners.push_back(Point2d(IMAGE_WIDTH, 0));				// TR
	dstCorners.push_back(Point2d(IMAGE_WIDTH, IMAGE_HEIGHT));	// BR
	dstCorners.push_back(Point2d(0, IMAGE_HEIGHT));				// BL

	// perform rectification
	Mat M = getPerspectiveTransform(corners, dstCorners);
	Mat_<Vec3b> imgWarped;
	warpPerspective(imgResizedColor, imgWarped, M, Size(500, 500));

	Mat_<Vec3b> imgNoBorder = imgWarped(Rect{
		BORDER_SIZE,						// x
		BORDER_SIZE,						// y
		IMAGE_WIDTH - 2 * BORDER_SIZE,		// width
		IMAGE_HEIGHT - 2 * BORDER_SIZE		// height
		});

	// "corner cells"
	Mat_<Vec3b> imgCell40 = extractCell(4, 0, imgNoBorder);
	Mat_<Vec3b> imgCell47 = extractCell(4, 7, imgNoBorder);
	Mat_<Vec3b> imgCell70 = extractCell(7, 0, imgNoBorder);
	Mat_<Vec3b> imgCell77 = extractCell(7, 7, imgNoBorder);

	if (SHOW_IMAGES)
	{
		imshowMutex->lock();
		if (CONCAT_IMAGES)
		{
			Mat_<Vec3b> wholeImages;
			Mat_<Vec3b> filler = Mat_<Vec3b>::zeros(imgWarped.rows - imgNoBorder.rows, imgNoBorder.cols);  // padding for imgNoBorder height
			Mat_<Vec3b> imgNoBorderPadded;
			cv::vconcat(imgNoBorder, filler, imgNoBorderPadded);
			cv::hconcat(imgWarped, imgNoBorderPadded, wholeImages);

			Mat_<Vec3b> cellImagesRow1;	
			Mat_<Vec3b> cellImagesRow2;
			Mat_<Vec3b> cellImages;

			cv::hconcat(imgCell40, imgCell47, cellImagesRow1);
			cv::hconcat(imgCell70, imgCell77, cellImagesRow2);
			cv::vconcat(cellImagesRow1, cellImagesRow2, cellImages);

			imshow("wholeImages", wholeImages);
			imshow("cellImages", cellImages);

			moveWindow("wholeImages", 50, 50);
			moveWindow("cellImages", 50, 50);
		}
		else
		{
			imshow("imgWarped", imgWarped);
			imshow("imgNoBorder", imgNoBorder);
			imshow("imgCell40", imgCell40);
			imshow("imgCell47", imgCell47);
			imshow("imgCell70", imgCell70);
			imshow("imgCell77", imgCell77);
		}
		waitKey();
		imshowMutex->unlock();
	}
}