#include "../../headers/configuration/Configurer.h"


Configurer::Configurer(CameraSide cameraSide, std::shared_ptr<QMutex> imshowMutex)
{
	this->cameraSide = cameraSide;
	this->imshowMutex = imshowMutex;
	configured = false; 
}


int Configurer::configure(Mat_<Vec3b> img, bool isTest, bool showImages, bool concatImages)
{
	// resize
	Mat_<Vec3b> imgResizedColor;
	resize(img, imgResizedColor, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	// convert to grayscale
	Mat_<uchar> imgResizedGrayscale;
	cv::cvtColor(imgResizedColor, imgResizedGrayscale, cv::COLOR_BGR2GRAY);

	// filter to reduce noise
	Mat_<uchar> imgGauss = gaussianFilter(imgResizedGrayscale, 7);

	// get binary image from filtered image
	Mat_<uchar> imgBinary = grayscaleToBinary(imgGauss, binaryThreshold);

	// perform closing on binary image to remove unnecessarry details
	uchar selPattern[DEFAULT_CLOSING_SIZE * DEFAULT_CLOSING_SIZE] = { 0 };
	const cv::Mat_<uchar> sel = cv::Mat(7, 7, CV_8UC1, selPattern);
	Mat_<uchar> imgClosed = closing(imgBinary, sel);

	// perform canny edge detection on closed image
	Mat_<uchar> imgCanny = canny(imgClosed);

	// perform hough on the image resulted from canny
	std::vector<Point2i> points = getPointsFromBinary(imgCanny);
	std::vector<lineRoTheta> lines = hough(
		points,
		imgResizedColor,
		DEFAULT_HOUGH_RO_STEP_SIZE,
		DEFAULT_HOUGH_THETA_STEP_SIZE,
		DEFAULT_HOUGH_WINDOW_SIZE,
		DEFAULT_HOUGH_NUMBER_OF_LINES
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

	if (!isTest)
	{
		// order is important
		corners.clear();
		corners.push_back(cornerChessboardTL);
		corners.push_back(cornerChessboardTR);
		corners.push_back(cornerChessboardBR);
		corners.push_back(cornerChessboardBL);

		// set flag
		configured = true;
	}

	// visualize
	if (showImages)
	{
		imshowMutex->lock();
		if (concatImages)
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


int Configurer::cropAndLabel(Mat_<Vec3b> imgOriginal, QVector<QString> encodings, bool isTest, bool concatImages,
	int borderTop, int borderRight, int borderBottom, int borderLeft)
{
	Mat_<Vec3b> imgWarped, imgNoBorder;
	if (warpAndRemoveBorder(imgOriginal, imgWarped, imgNoBorder, borderTop, borderRight, borderBottom, borderLeft) != 0)
	{
		return 1;
	}

	std::vector<std::pair<Mat_<Vec3b>, std::string>> imagesAndPathsToShowIfTest;

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

			// extract based on camera's coord system (i,j), save with name based on main coord system (row,col)
			Mat_<Vec3b> imgCell = extractCell(i, j, imgNoBorder, borderTop, borderRight, borderBottom, borderLeft);
			if (isTest)
			{
				// corner images usually provide better overview
				if (i == 4 && j == 0 || i == 4 && j == 7 || i == 7 && j == 0 || i == 7 && j == 7)
				{
					imagesAndPathsToShowIfTest.push_back(std::pair<Mat_<Vec3b>, std::string>(imgCell, FileHandler::boardImageName(row, col)));
				}
			}
			else
			{
				std::string folder = TEMPORARY_FOLDER_PATH + std::string("\\") + encodings[row * 8 + col].toStdString();
				std::string path;
				int ret = FileHandler::saveImage(imgCell, folder, path, true, FileHandler::boardImageName(row, col));
				if (ret != 0)
				{
					SPDLOG_ERROR("Saving cell image {} failed!", path);
					return 2;
				}
			}
		}
	}

	if (isTest)
	{
		imshowMutex->lock();
		if (concatImages)
		{
			Mat_<Vec3b> wholeImages;
			Mat_<Vec3b> filler = Mat_<Vec3b>::zeros(imgWarped.rows - imgNoBorder.rows, imgNoBorder.cols);  // padding for imgNoBorder height
			Mat_<Vec3b> imgNoBorderPadded;
			cv::vconcat(imgNoBorder, filler, imgNoBorderPadded);
			cv::hconcat(imgWarped, imgNoBorderPadded, wholeImages);

			Mat_<Vec3b> cellImagesRow1;
			Mat_<Vec3b> cellImagesRow2;
			Mat_<Vec3b> cellImages;

			cv::hconcat(imagesAndPathsToShowIfTest[0].first, imagesAndPathsToShowIfTest[1].first, cellImagesRow1);
			cv::hconcat(imagesAndPathsToShowIfTest[2].first, imagesAndPathsToShowIfTest[3].first, cellImagesRow2);
			cv::vconcat(cellImagesRow1, cellImagesRow2, cellImages);

			imshow("wholeImages", wholeImages);
			imshow("cellImages", cellImages);

			cv::moveWindow("wholeImages", 50, 50);
			cv::moveWindow("cellImages", 50, 50);
		}
		else
		{
			imshow("imgWarped", imgWarped);
			imshow("imgNoBorder", imgNoBorder);
			for (auto imageAndPathsToShow : imagesAndPathsToShowIfTest)
			{
				imshow(imageAndPathsToShow.second, imageAndPathsToShow.first);
			}
		}
		waitKey();
		imshowMutex->unlock();
	}

	return 0;
}


int Configurer::prepareCellImages(Mat_<Vec3b> imgOriginal, int borderTop, int borderRight, int borderBottom, int borderLeft)
{
	Mat_<Vec3b> imgWarped, imgNoBorder;
	if (warpAndRemoveBorder(imgOriginal, imgWarped, imgNoBorder, borderTop, borderRight, borderBottom, borderLeft) != 0)
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


			// extract based on camera's coord system (i,j), save with name based on main coord system (row,col)
			Mat_<Vec3b> imgCell = extractCell(i, j, imgNoBorder, borderTop, borderRight, borderBottom, borderLeft);
			std::string path;
			int ret = FileHandler::saveImage(imgCell, BOARD_FOLDER_PATH, path, false, FileHandler::boardImageName(row, col));
			if (ret != 0)
			{
				SPDLOG_ERROR("Saving cell image {} failed!", path);
				return 2;
			}
		}
	}

	return 0;
}


Mat_<Vec3b> Configurer::extractCell(int i, int j, Mat_<Vec3b> img, int borderTop, int borderRight, int borderBottom, int borderLeft)
{
	int width = cellWidth(borderRight, borderLeft);
	int height = cellHeight(borderTop, borderBottom);

	return img(Rect{
		width* j,			// x
		height* (i - 1),	// y - start from cell above
		width,				// width
		height * 2			// height - two cells
		});
}


void Configurer::leftToMain(int lr, int lc, int& mr, int& mc)
{
	mr = lc;
	mc = 7 - lr;
}


void Configurer::rightToMain(int rr, int rc, int& mr, int& mc)
{
	mr = 7 - rc;
	mc = rr;
}


int Configurer::warpAndRemoveBorder(Mat_<Vec3b> imgOriginal, Mat_<Vec3b>& imgWarped, Mat_<Vec3b>& imgNoBorder,
	int borderTop, int borderRight, int borderBottom, int borderLeft)
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
		borderLeft,								// x
		borderTop,									// y
		IMAGE_WIDTH - borderLeft - borderRight,	// width
		IMAGE_HEIGHT - borderTop - borderBottom	// height
		});

	return 0;
}


void Configurer::drawCrossColor(Mat_<Vec3b> img, Point2i p, int diameter, Vec3b color)
{
	int halfDiameter = diameter / 2;
	int xl = p.x - halfDiameter;	// x left
	int xr = p.x + halfDiameter;	// x right
	int yt = p.y - halfDiameter;	// y top
	int yb = p.y + halfDiameter;	// y bottom

	Point2i l(xl, p.y);
	Point2i r(xr, p.y);
	Point2i t(p.x, yt);
	Point2i b(p.x, yb);

	line(img, l, r, color);
	line(img, t, b, color);
}


std::vector<Point2i> Configurer::getPointsFromBinary(Mat_<uchar> img, int objectPixelColor)
{
	std::vector<Point2i> points;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img(i, j) == objectPixelColor)
			{
				points.push_back(Point2i(j, i));
			}
		}
	}

	return points;
}


double Configurer::euclideanDistance(Point2i a, Point2i b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
