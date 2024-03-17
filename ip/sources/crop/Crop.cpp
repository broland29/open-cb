#include "../../headers/crop/Crop.h"

namespace fs = std::experimental::filesystem;


void Crop::leftToMain(int lr, int lc, int& mr, int& mc)
{
	mr = lc;
	mc = 7 - lr;
}


void Crop::rightToMain(int rr, int rc, int& mr, int& mc)
{
	mr = 7 - rc;
	mc = rr;
}

int Crop::warpAndRemoveBorder(Mat_<Vec3b> imgOriginal, std::vector<Point2f> corners, bool configured, Mat_<Vec3b>& imgWarped, Mat_<Vec3b>& imgNoBorder)
{
	if (!configured)
	{
		SPDLOG_ERROR("Must run configure first!");
		return 1;
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
	warpPerspective(imgResizedColor, imgWarped, M, Size(500, 500));

	imgNoBorder = imgWarped(Rect{
		BORDER_SIZE,						// x
		BORDER_SIZE,						// y
		IMAGE_WIDTH - 2 * BORDER_SIZE,		// width
		IMAGE_HEIGHT - 2 * BORDER_SIZE		// height
		});

	return 0;
}

int Crop::sendToFolder(Mat_<Vec3b> img, std::vector<Point2f> corners, bool configured, CameraSide cameraSide, std::string board[8][8], int imageCount, std::string basePath)
{
	std::string path;															// final path
	std::string side = (cameraSide == CameraSide::LEFT) ? "le" : "ri";			// part of name as well

	Mat_<Vec3b> imgWarped, imgNoBorder;
	int result = warpAndRemoveBorder(img, corners, configured, imgWarped, imgNoBorder);
	if (result != 0)
	{
		return 1;
	}

	// left part of board detected by cam1, right part by cam2
	// however, each camera sees what's closer to it as "lower cells" (different perspective)
	for (int i = 4; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			// coordinate conversion to main coordinate system
			int row, col;
			if (cameraSide == CameraSide::LEFT)
			{
				leftToMain(i, j, row, col);
			}
			else
			{
				rightToMain(i, j, row, col);
			}

			path = basePath +
				std::string("\\") + board[row][col] + std::string("\\") +									// folder - encoding given in main coord system
				std::string("no") + std::to_string(imageCount) + side +										// image identification
				std::string("_row") + std::to_string(row) + std::string("_col") + std::to_string(col) +		// cell identification - only care about main coord system
				std::string(".jpeg");																		// extension

			// extract and save cell
			Mat_<Vec3b> imgCell = extractCell(i, j, imgNoBorder);  // extraction based on camera's coord system
			if (cv::imwrite(path, imgCell) == false)
			{
				SPDLOG_ERROR("Failed to save cell to {}", path);
				return 1;
			}
		}
	}

	return 0;
}

int Crop::sendToTrain(Mat_<Vec3b> img, std::vector<Point2f> corners, bool configured, CameraSide cameraSide, std::string board[8][8], int imageCount)
{
	return sendToFolder(img, corners, configured, cameraSide, board, imageCount, TRAIN_FOLDER_PATH);
}


int Crop::sendToTest(Mat_<Vec3b> img, std::vector<Point2f> corners, bool configured, CameraSide cameraSide, std::string board[8][8], int imageCount)
{
	return sendToFolder(img, corners, configured, cameraSide, board, imageCount, TEST_FOLDER_PATH);
}


int Crop::resetFolder(std::string basePath)
{
	std::string path = basePath;

	// delete whole train folder
	if (fs::remove_all(path) == -1)  // https://en.cppreference.com/w/cpp/filesystem/remove
	{
		SPDLOG_ERROR("Error when removing folder {}", path);
		return 1;
	}

	// re-create train folder
	if (fs::create_directory(path) == false)  // https://en.cppreference.com/w/cpp/filesystem/create_directory
	{
		SPDLOG_ERROR("Error when creating folder {}", path);
		return 2;
	}

	// re-create subdirectories
	for (std::string folder : folders)
	{
		path = basePath + std::string("\\") + folder;
		if (fs::create_directory(path) == false)  // https://en.cppreference.com/w/cpp/filesystem/create_directory
		{
			SPDLOG_ERROR("Error when creating folder {}", path);
			return 3;
		}
	}

	return 0;
}

int Crop::resetTrain()
{
	return resetFolder(TRAIN_FOLDER_PATH);
}


int Crop::resetTest()
{
	return resetFolder(TEST_FOLDER_PATH);
}


int Crop::configure(Mat_<Vec3b> imgOriginal, std::vector<Point2f> &corners, bool &configured, std::shared_ptr<QMutex> imshowMutex)
{
	// resize
	Mat_<Vec3b> imgResizedColor;
	resize(imgOriginal, imgResizedColor, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	// convert to grayscale
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
				SPDLOG_TRACE("Lines [{},{}] and [{},{}] are parallel!",
					lines[i].ro, lines[i].theta, lines[j].ro, lines[j].theta);
				continue;
			}

			if (!isInside(imgIntersections, intersection.y, intersection.x))
			{
				SPDLOG_TRACE("Lines [{},{}] and [{},{}] intersect outside of the image!",
					lines[i].ro, lines[i].theta, lines[j].ro, lines[j].theta);
				continue;
			}

			intersections.push_back(intersection);
			drawCrossColor(imgIntersections, intersection, 50, Vec3b(0, 0, 255));
			SPDLOG_TRACE("Lines [{},{}] and [{},{}] intersect in [{},{}]",
				lines[i].ro, lines[i].theta, lines[j].ro, lines[j].theta, intersection.x, intersection.y);
		}
	}

	SPDLOG_TRACE("Intersections:");
	for (Point2i intersection : intersections)
	{
		SPDLOG_TRACE("[{},{}]", intersection.x, intersection.y);
	}

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

	SPDLOG_TRACE("minX: [{},{}]", cornerChessboardTL.x, cornerChessboardTL.y);
	SPDLOG_TRACE("maxX: [{},{}]", cornerChessboardTR.x, cornerChessboardTR.y);
	SPDLOG_TRACE("minY: [{},{}]", cornerChessboardBL.x, cornerChessboardBL.y);
	SPDLOG_TRACE("maxY: [{},{}]", cornerChessboardBR.x, cornerChessboardBR.y);

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

	// set flag
	configured = true;

	// visualize
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

	return 0;
}

int Crop::getImage(Mat_<Vec3b> imgOriginal, std::vector<Point2f> corners, bool configured, std::shared_ptr<QMutex> imshowMutex)
{
	Mat_<Vec3b> imgWarped, imgNoBorder;
	int result = warpAndRemoveBorder(imgOriginal, corners, configured, imgWarped, imgNoBorder);
	if (result != 0)
	{
		return 1;
	}

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