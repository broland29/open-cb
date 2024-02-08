#include "stdafx.h"
#include "Util.h"
#include <iostream>
/*
#include "Util.h"
#include "Conversion.h"
#include "Morphological.h"
#include "Hough.h"
#include "Canny.h"
#include "Filter.h"
#include "KNearestNeighbors.h"
#include <thread>
*/

#define BINARY_THRESHOLD		150
#define CLOSING_SIZE			6
#define HOUGH_RO_STEP_SIZE		1
#define HOUGH_THETA_STEP_SIZE	1
#define HOUGH_WINDOW_SIZE		3
#define HOUGH_NUMBER_OF_LINES	10

#define TEST_FILE_PATH "Images\\img002.jpg"

#define SHOW_IMAGES true
#define GENERATE_IMAGES false
#define CLASSIFY false

// already included?
//#include <winsock2.h>
//#include <ws2tcpip.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55555
#include "Client.h"

/*
Mat_<Vec3b> extractCell(int i, int j, Rect ROI, Mat_<Vec3b> imgWarped)
{
	int stepX = ROI.width / 8;
	int stepY = ROI.height / 8;

	Rect ROICell(ROI.x + stepX * i, ROI.y + stepY * (j - 1), stepX, stepY * 2);
	Mat_<Vec3b> imgCell = imgWarped(ROICell);

	return imgCell;
}

void screenshotThreadFunction(VideoCapture videoCapture)
{
	Mat_<Vec3b> imgFrame;
	videoCapture.read(imgFrame);
	imshow("screenshotThread", imgFrame);
	waitKey();
}

void videoThreadFunction()
{
	VideoCapture videoCapture;
	//videoCapture.open("http://192.168.100.34:8080/video");
	videoCapture.open(0);

	if (!videoCapture.isOpened())
	{
		std::cout << "Could not open camera!" << std::endl;
		return;
	}

	Mat_<Vec3b> imgFrame;
	int keyPressed;
	for (;;)
	{
		videoCapture.read(imgFrame);

		if (imgFrame.empty())
		{
			std::cout << "Empty frame!" << std::endl;
			break;
		}
		imshow("videoThread", imgFrame);

		keyPressed = waitKey(5);
		if (keyPressed == 'e')
		{
			std::cout << "e pressed" << std::endl;
			break;
		}
		else if (keyPressed == 's')
		{
			std::cout << "s pressed" << std::endl;
			std::thread screenshotThread(screenshotThreadFunction, videoCapture);
			screenshotThread.detach();  // https://stackoverflow.com/questions/39742155/c-single-threaded-works-on-new-thread-it-calls-abort
		}
	}
}
*/



int testCameras()
{
	// 1 seems to be main camera
	cv::VideoCapture cameraOne(0);
	cv::VideoCapture cameraTwo(2);

	if (!cameraOne.isOpened())
	{
		std::cout << "Could not open camera one!" << std::endl;
		return 1;
	}
	if (!cameraTwo.isOpened())
	{
		std::cout << "Could not open camera one!" << std::endl;
		return 1;
	}

	Mat_<Vec3b> imgOriginalColorOne, imgOriginalColorTwo;
	while (true)
	{
		cameraOne >> imgOriginalColorOne;
		cameraTwo >> imgOriginalColorTwo;

		Mat_<Vec3b> imgResizedColorOne, imgResizedColorTwo;
		resize(imgOriginalColorOne, imgResizedColorOne, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));
		resize(imgOriginalColorTwo, imgResizedColorTwo, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

		imshow("imgResizedColorOne", imgResizedColorOne);
		imshow("imgResizedColorTwo", imgResizedColorTwo);

		int c = waitKey(40);
		if (27 == char(c))  // esc
		{
			std::cout << "exiting" << std::endl;
			break;
		}
		if (char(c) == 'i')
		{
			std::cout << "saving images" << std::endl;
			imwrite(PATH_IMG_CAM_ONE, imgResizedColorOne);
			imwrite(PATH_IMG_CAM_TWO, imgResizedColorTwo);
		}
	}
	cameraOne.release();
	cameraTwo.release();
}

int main()
{
	//testCameras();
	return client_main();
	/*
	std::thread videoThread(videoThreadFunction);
	videoThread.join();
	*/
	/*
	//videoCapture.set(CAP_PROP_BUFFERSIZE, 1);

	namedWindow("imgFrame", 1);
	char fileName[100];
	int count = 0;
	int keyPressed = 'I';

	//std::thread threads[2];
	while (1)
	{
		switch (keyPressed)
		{
		case ' ':
			std::cout << "Space pressed!" << std::endl;
			sprintf(fileName, "CameraTest\\Frame_%04d.jpeg", count++);
			imwrite(fileName, imgFrame);
			break;
		case 'e':
			std::cout << "E pressed!" << std::endl;
			return 0;
		}

		std::cout << "Read frame!" << std::endl;
		videoCapture >> imgFrame;
		
		imshow("imgFrame", imgFrame);

		keyPressed = waitKey(0);
	}
	*/
	/*
	if (CLASSIFY)
	{
		goto _skip;
	}

	int fileNo = 1;
	char fileName[100];
	while (true)
	{
		sprintf(fileName, "Images\\img_%02d.jpeg", fileNo);

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

		// remove border
		int roiX = 50;
		int roiY = 60;
		int roiWidth = 400;
		int roiHeight = 370;
		Rect ROI(roiX, roiY, roiWidth, roiHeight);
		Mat_<Vec3b> imgROI = imgWarped(ROI);

		Mat_<Vec3b> imgCell00 = extractCell(0, 0, ROI, imgWarped);
		Mat_<Vec3b> imgCell77 = extractCell(7, 7, ROI, imgWarped);
		Mat_<Vec3b> imgCell34 = extractCell(3, 4, ROI, imgWarped);

		if (GENERATE_IMAGES)
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					Mat_<Vec3b> imgCell = extractCell(i, j, ROI, imgWarped);
					sprintf(fileName, "Cells\\cell_%02d%02d%02d.jpeg", fileNo, i, j);
					imwrite(fileName, imgCell);
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
			imshow("imgROI", imgROI);
			imshow("imgCell00", imgCell00);
			imshow("imgCell77", imgCell77);
			imshow("imgCell34", imgCell34);

			waitKey();
		}

		fileNo++;
	}

_skip:
	if (CLASSIFY)
	{
		const int C = 13;  // number of classes
		const std::array<std::string, C> classes = {
			"black_bishop",
			"black_king",
			"black_knight",
			"black_pawn",
			"black_queen",
			"black_rook",
			"free",
			"white_bishop",
			"white_king",
			"white_knight",
			"white_pawn",
			"white_queen",
			"white_rook"
		};
		std::map<std::string, std::vector<Mat_<Vec3b>>> images;

		// insert empty vectors for each class
		for (int i = 0; i < C; i++)
		{
			images.insert(
				std::pair<std::string, std::vector<Mat_<Vec3b>>>(
					classes[i],
					std::vector<Mat_<Vec3b>>()
			));
		}

		int fileNo;
		char fileName[200];
		for (int i = 0; i < C; i++)
		{
			fileNo = 1;

			// need char array, have string
			char dirName[20];
			strcpy(dirName, classes[i].c_str());

			while (true)
			{
				sprintf(fileName, "CellsDemo/%s/%s_%05d.jpeg", dirName, dirName, fileNo++);

				//std::cout << fileName << std::endl;
				Mat_<Vec3b> img = imread(fileName, IMREAD_COLOR);
				if (img.empty())
				{
					break;  // no more images of class
				}
				//std::cout << fileName << std::endl;

				// store image
				images[classes[i]].push_back(img);
			}
		}

		std::map<std::string, std::vector<Mat_<Vec3b>>> trainImages;
		std::map<std::string, std::vector<Mat_<Vec3b>>> testImages;
		for (int i = 0; i < C; i++)
		{
			trainImages.insert(
				std::pair<std::string, std::vector<Mat_<Vec3b>>>(
					classes[i],
					std::vector<Mat_<Vec3b>>()
					));
			testImages.insert(
				std::pair<std::string, std::vector<Mat_<Vec3b>>>(
					classes[i],
					std::vector<Mat_<Vec3b>>()
					));
		}
		int trainSplit = 80;
		for (int i = 0; i < C; i++)
		{
			std::vector<Mat_<Vec3b>> imagesOfAClass = images[classes[i]];
			int firstTestImageIndex = (imagesOfAClass.size() * trainSplit) / 100;

			int j = 0;
			while (j < firstTestImageIndex)
			{
				trainImages[classes[i]].push_back(imagesOfAClass.at(j));
				j++;
			}
			while (j < imagesOfAClass.size())
			{
				testImages[classes[i]].push_back(imagesOfAClass.at(j));
				j++;
			}
		}

		// instantiate classifier (includes training)
		KNearestNeighborsClassifier classifier(trainImages, 5);

		// testing phase
		Mat_<double> confusionMatrix(C, C, 0.0);  // row: predicted class  col: actual class
		for (auto const& pair : testImages)
		{
			std::string actualClass = pair.first;           // class
			std::vector<Mat_<Vec3b>> images = pair.second;  // vector of images
			for (Mat_<Vec3b> image : images)
			{
				std::string predictedClass = classifier.classify(image);

				int predictedClassIndex, actualClassIndex;
				for (int i = 0; i < C; i++)
				{
					if (actualClass == classes[i])
					{
						actualClassIndex = i;
					}
					if (predictedClass == classes[i])
					{
						predictedClassIndex = i;
					}
				}

				confusionMatrix(actualClassIndex, predictedClassIndex)++;
			}
		}

		double correct = 0;
		double wrong = 0;
		for (int i = 0; i < C; i++)
		{
			for (int j = 0; j < C; j++)
			{
				if (i == j)
				{
					correct += confusionMatrix(i, j);
				}
				else
				{
					wrong += confusionMatrix(i, j);
				}
			}
		}
		double accuracy = correct / (correct + wrong);

		std::cout << "Train images:" << std::endl;
		int total = 0;
		for (auto const& pair : trainImages)
		{
			total += pair.second.size();
			std::cout << pair.first << ": " << pair.second.size() << std::endl;
		}
		std::cout << "Total: " << total << std::endl << std::endl;

		total = 0;
		std::cout << "Test images:" << std::endl;
		for (auto const& pair : testImages)
		{
			total += pair.second.size();
			std::cout << pair.first << ": " << pair.second.size() << std::endl;
		}
		std::cout << "Total: " << total << std::endl << std::endl;

		std::cout << "Confusion matrix:" << std::endl;
		std::cout << "Classes: ";
		for (int i = 0; i < classes.size(); i++)
		{
			std::cout << " (" << i << ":" << classes[i] << ")";
		}
		std::cout << std::endl << confusionMatrix << std::endl << std::endl;

		std::cout << "Accuracy: " << accuracy << std::endl;
	}
	*/
	return 0;
}