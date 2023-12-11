#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <windows.h>

using namespace cv;



#define PI 3.14159265

#define BINARY_THRESHOLD 100
#define IMAGE_WIDTH 500
#define IMAGE_HEIGHT 500
#define HOUGH_RO_STEP_SIZE 10
#define HOUGH_THETA_STEP_SIZE 10
#define HOUGH_WINDOW_SIZE 5

bool isInside(const cv::Mat& img, int i, int j) {
	return (i >= 0 && i < img.rows) && (j >= 0 && j < img.cols);
}

Mat_<uchar> grayscaleToBinary(Mat_<uchar> img, int threshold) {
	Mat_<uchar> grayImg(img.rows, img.cols);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img(i, j) < threshold) {
				grayImg(i, j) = 0;		// black
			}
			else {
				grayImg(i, j) = 255;	// white
			}
		}
	}

	return grayImg;
}

cv::Mat_<cv::Vec3b> binaryToColor(cv::Mat_<uchar> img) {
	cv::Mat_<cv::Vec3b> imgRes(img.rows, img.cols);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img(i, j) == 0) {
				imgRes(i, j) = cv::Vec3b(0, 0, 0);
			}
			else {
				imgRes(i, j) = cv::Vec3b(255, 255, 255);
			}
		}
	}

	return imgRes;
}

std::vector<cv::Point2i> borderTracing(Mat_<uchar> img) {
	std::vector<cv::Point2i> points;	// points of border

	// search for first pixel
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img(i, j) == 0) {
				points.push_back(cv::Point2i(j, i));
				goto found;
			}
		}
	}
	std::cout << "Point not found!" << std::endl;
	// TODO - how to handle?
found:

	// index offsets for 8-neighborhood (counter clockwise)
	int di[] = { 0, -1, -1, -1,  0,  1, 1, 1 };
	int dj[] = { 1,  1,  0, -1, -1, -1, 0, 1 };
	int pi, pj;				// previous indices
	int ci, cj;				// current indices
	int currDir;			// current direction (step we are about to make)
	int prevDir = 7;		// direction from previous point to current (8-connectivity => initially 7)
	cv::Point2i prevPoint;	// previous point

	int count = 0;  // counter for safety

	// traverse the contour
	do {
		prevPoint = points.at(points.size() - 1);
		pi = prevPoint.y;
		pj = prevPoint.x;

		// calculate beginning direction
		if (prevDir % 2 == 0) {
			currDir = (prevDir + 7) % 8;
		}
		else {
			currDir = (prevDir + 6) % 8;
		}

		// search for neighbor object pixel from beginning direction in counter clockwise direction
		for (int i = 0; i < 8; i++) {
			ci = pi + di[currDir];
			cj = pj + dj[currDir];

			if (img(ci, cj) == 0) {
				points.push_back(cv::Point2i(cj, ci));
				prevDir = currDir;
				break;
			}

			currDir = (currDir + 1) % 8;
		}

		count++;
		/*	if (count == (IMAGE_WIDTH + IMAGE_HEIGHT) * 50) // chess table border theoretically should not be larger than image (2 * (width + height))
			{
				std::cout << "Reached iteration limit. Border probably is not continuous" << std::endl;
				break;
			}*/
	} while (points.size() < 3 ||							// cannot perform next checks, so continue
		points.at(points.size() - 1) != points.at(1) ||		// last point equal to second
		points.at(points.size() - 2) != points.at(0));		// before last equal to first

	return points;
}

// Perform erosion on img, with structuring element sel
cv::Mat_<uchar> erosion(cv::Mat_<uchar> img, cv::Mat_<uchar> sel) {
	cv::Mat_<uchar> erosionImg(img.rows, img.cols, 255);

	// iterate original pixels
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			// only consider object pixels from original
			if (img(i, j) != 0) {
				continue;
			}

			// iterate pixels given by structuring element
			//		"If the structuring element covers any background pixel,
			//		 the pixel in the result image keeps its background label."
			for (int u = 0; u < sel.rows; u++) {
				for (int v = 0; v < sel.cols; v++) {
					// only consider object pixels from structuring element
					if (sel(u, v) != 0) {
						continue;
					}

					// offset structuring element's pixel
					int i2 = u - sel.rows / 2 + i;
					int j2 = v - sel.cols / 2 + j;

					if (isInside(erosionImg, i2, j2) && img(i2, j2) != 0) {
						goto skip;
					}
				}
			}
			erosionImg(i, j) = 0;
		skip:;
		}
	}

	return erosionImg;
}

// Perform dilation on img, with structuring element sel
cv::Mat_<uchar> dilation(cv::Mat_<uchar> img, cv::Mat_<uchar> sel) {
	cv::Mat_<uchar> dilationImg(img.rows, img.cols, 255);

	// iterate original pixels
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			// only consider object pixels from original
			if (img(i, j) != 0) {
				continue;
			}

			// iterate pixels given by structuring element
			//		"If the origin of the structuring element coincides with an object pixel in the image,
			//		 label all pixels covered by the structuring element as object pixels in the result image."
			for (int u = 0; u < sel.rows; u++) {
				for (int v = 0; v < sel.cols; v++) {
					// only consider object pixels from structuring element
					if (sel(u, v) != 0) {
						continue;
					}

					// offset structuring element's pixel
					int i2 = u - sel.rows / 2 + i;
					int j2 = v - sel.cols / 2 + j;

					if (isInside(dilationImg, i2, j2)) {
						dilationImg(i2, j2) = 0;
					}
				}
			}
		}
	}

	return dilationImg;
}


// "Opening consists of an erosion followed by a dilation and can be used to eliminate
//  all pixels in regions that are too small to contain the structuring element."
cv::Mat_<uchar> opening(cv::Mat_<uchar> img, const cv::Mat_<uchar>& sel) {
	return dilation(erosion(img, sel), sel);
}


cv::Mat_<uchar> closing(cv::Mat_<uchar> img, const cv::Mat_<uchar>& sel) {
	return erosion(dilation(img, sel), sel);
}

// Draw a cross on image img, "around" point p, with given diameter (and optionally color)
void drawCross(cv::Mat_<uchar> img, cv::Point2i p, int diameter, int color = 255) {
	// calculate potential end coordinates of cross
	int halfDiameter = diameter / 2;
	int xl = p.x - halfDiameter;	// x left
	int xr = p.x + halfDiameter;	// x right
	int yt = p.y - halfDiameter;	// y top
	int yb = p.y + halfDiameter;	// y bottom

	// make sure end points inside img
	if (xl < 0) {
		xl = 0;
	}
	if (xr > img.cols) {
		xr = img.cols;
	}
	if (yt < 0) {
		yt = 0;
	}
	if (yb > img.rows) {
		yb = img.rows;
	}

	// define points for drawing line and draw lines
	cv::Point2i l(xl, p.y);
	cv::Point2i r(xr, p.y);
	cv::Point2i t(p.x, yt);
	cv::Point2i b(p.x, yb);
	cv::line(img, l, r, color);
	cv::line(img, t, b, color);
}

// Draw a cross on image img, "around" point p, with given diameter (and optionally color)
void drawCrossColor(cv::Mat_<Vec3b> img, cv::Point2i p, int diameter, Vec3b color) {
	// calculate potential end coordinates of cross
	int halfDiameter = diameter / 2;
	int xl = p.x - halfDiameter;	// x left
	int xr = p.x + halfDiameter;	// x right
	int yt = p.y - halfDiameter;	// y top
	int yb = p.y + halfDiameter;	// y bottom

	cv::Point2i l(xl, p.y);
	cv::Point2i r(xr, p.y);
	cv::Point2i t(p.x, yt);
	cv::Point2i b(p.x, yb);
	cv::line(img, l, r, color);
	cv::line(img, t, b, color);
}




// return how many times we need to increment to get from min to max with stepSize increment
int getStepCount(int min, int max, int stepSize)
{
	int range = max - min + 1;
	if (range % stepSize == 0)
	{
		return max / stepSize;
	}
	else
	{
		return max / stepSize + 1;
	}
}

// map input from [inputStart, inputEnd] to [outputStart, outputEnd]
// https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another
int mapToInterval(int input, int inputStart, int inputEnd, int outputStart, int outputEnd)
{
	double slope = 1.0 * (outputEnd - outputStart) / (inputEnd - inputStart);
	return floor(outputStart + slope * (input - inputStart) + 0.5);
}


struct Peak {
	int theta, ro, hval;
	bool operator < (const Peak& o) const {
		return hval > o.hval;
	}
};

// https://math.stackexchange.com/questions/1992153/given-two-lines-each-defined-using-hesse-normal-form-find-the-intersection-poin
Point2i getIntersection(int thetaAngle1, int ro1, int thetaAngle2, int ro2)
{
	float thetaRadian1 = thetaAngle1 * PI / 180;
	float thetaRadian2 = thetaAngle2 * PI / 180;
	float a = cos(thetaRadian1);
	float b = sin(thetaRadian1);
	float c = ro1;
	float d = cos(thetaRadian2);
	float e = sin(thetaRadian2);
	float f = ro2;
	return Point2i(
		(c * e - b * f) / (a * e - b * d),
		(a * f - c * d) / (a * e - b * d));
}

// returns corner points
Mat_<Vec3b> hough(std::vector<Point2i> points, cv::Mat_<cv::Vec3b> imgBorder)
{
	int D = sqrt(imgBorder.rows * imgBorder.rows + imgBorder.cols + imgBorder.cols);	// diagonal, maximum value of ro
	Mat_<int> Hough(D + 1, 360, CV_32SC1);												// Hough accumulator (int since char may overflow)
	Hough.setTo(0);

	for (Point2i point : points)
	{
		// std::cout << point.x << " " << point.y << std::endl;
		for (int thetaAngle = 0; thetaAngle < 360; thetaAngle++)
		{
			/*
			// skip for thetas which give lines out of image
			if (thetaAngle >= 180 && thetaAngle <= 270)
			{
				continue;
			}*/

			float thetaRadian = thetaAngle * PI / 180;  // sin and cos expect radians; float!
			int ro = point.x * cos(thetaRadian) + point.y * sin(thetaRadian);
			if (ro >= 0 && ro <= D)
			{
				Hough(ro, thetaAngle)++;  // thetaAngle!
			}
		}
	}

	int maxHough = 0;
	for (int i = 0; i < Hough.rows; i++)
	{
		for (int j = 0; j < Hough.cols; j++)
		{
			if (Hough(i, j) > maxHough)
			{
				maxHough = Hough(i, j);
			}
		}
	}
	Mat_<uchar> houghImg;  // to be used only for visualization!
	Hough.convertTo(houghImg, CV_8UC1, 255.f / maxHough);
	imshow("houghImg", houghImg);

	std::vector<Peak> peaks;
	int w = 3;  // window size
	for (int i = 0; i < Hough.rows; i++)
	{
		for (int j = 0; j < Hough.cols; j++)
		{
			int localMax = 0;
			for (int u = -w / 2; u <= w / 2; u++)
			{
				for (int v = -w / 2; v <= w / 2; v++)
				{
					int ci = i + u;
					int cj = j + v;

					// skip invalid ro
					if (ci < 0 || ci >= Hough.rows)
					{
						continue;
					}

					// to wrap around invalid theta
					cj = (cj + 360) % 360;

					if (Hough(ci, cj) > localMax)
					{
						localMax = Hough(ci, cj);
					}
				}
			}
			if (Hough(i, j) == localMax)
			{
				peaks.push_back(Peak{ j, i, localMax });
			}
		}
	}

	std::sort(peaks.begin(), peaks.end());

	Mat_<Vec3b> linesImg = imgBorder;
	int k = 30;
	if (k > peaks.size())
	{
		k = peaks.size();
	}

	for (int i = 0; i < k; i++)
	{
		float thetaRadian = peaks[i].theta * PI / 180;  // sin, cos expects radians
		int ro = peaks[i].ro;
		// std::cout << thetaRadian << " " << ro << std::endl;
		Point2i A, B;
		if (thetaRadian > -(PI / 4) && thetaRadian < PI / 4)  // avoids division by 0
		{
			A = Point2i(0, ro / sin(thetaRadian));  // take x = 0
			B = Point2i(linesImg.cols, (ro - linesImg.cols * cos(thetaRadian)) / sin(thetaRadian));
		}
		else
		{
			A = Point2i(ro / cos(thetaRadian), 0);
			B = Point2i((ro - linesImg.rows * sin(thetaRadian)) / cos(thetaRadian), linesImg.rows);
		}
		line(linesImg, A, B, Scalar(0, 255, 0), 3);
		// std::cout << A << " " << B << std::endl;
	}

	/*
	std::vector<Point2i> corners;
	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			Point2i point = getIntersection(peaks[i].theta, peaks[i].ro, peaks[j].theta, peaks[j].ro);
			if (!isInside(linesImg, point.y, point.x))
			{
				continue;
			}
			corners.push_back(point);
		}
	}
	for (Point2i corner : corners)
	{
		drawCrossColor(linesImg, corner, 50, Vec3b(255, 0, 0));
	}


	imshow("linesImg", linesImg);

	return corners;
	*/
	return linesImg;
}

bool comparePointY(Point2i p1, Point2i p2)
{
	return p1.y < p2.y;
}

void calculateAverageColor(Mat_<Vec3b> img, std::vector<Point2i> cornersOfROI)
{
	Mat_<uchar> mask = Mat(img.rows, img.cols, CV_8UC1);

	sort(cornersOfROI.begin(), cornersOfROI.end(), comparePointY);

	Mat_<Vec3b> imgROI = img;
	line(imgROI, cornersOfROI[0], cornersOfROI[1], Scalar(0, 255, 0), 3);
	line(imgROI, cornersOfROI[0], cornersOfROI[2], Scalar(0, 255, 0), 3);
	line(imgROI, cornersOfROI[3], cornersOfROI[1], Scalar(0, 255, 0), 3);
	line(imgROI, cornersOfROI[3], cornersOfROI[2], Scalar(0, 255, 0), 3);

	// index offsets for 8-neighborhood
	int di[8] = { -1, -1, -1, 0, 1, 1,  1,  0 };
	int dj[8] = { -1,  0,  1, 1, 1, 0, -1, -1 };

	// flood from middle of image
	std::queue<Point2i> queue;
	queue.push(Point2i{ imgROI.cols / 2, imgROI.rows / 2 });
	while (!queue.empty())
	{
		Point2i point = queue.front();
		queue.pop();

		// for each neighbor
		for (int k = 0; k < 8; k++) {
			int ni = point.y + di[k];
			int nj = point.x + dj[k];

			if (isInside(imgROI, ni, nj)) {
				if (imgROI(ni, nj) != Vec3b(0, 255, 0)) {  // todo - veeery little chance corner case: image contains exactly this green
					imgROI(ni, nj) = Vec3b(0, 255, 0);
					queue.push(Point2i(nj, ni));
				}
			}
		}
	}

	imshow("imgROI", imgROI);
}

// normalization
// convImg - the result of the convolution
// H - the kernel used for the convolution of convImg
Mat_<uchar> floatToGrayscale(Mat_<float> convImg, Mat_<float> H)
{
	// map [a, b] to [c, d]
	float a = 0;		// min conv value we can get -> every positive taken 0 and every negative taken 255 (since original image grayscale)
	float b = 0;		// max conv value we can get -> opposite approach
	float c = 0;		// min grayscale
	float d = 255;		// max grayscale

	for (int i = 0; i < H.rows; i++) {
		for (int j = 0; j < H.cols; j++) {
			if (H(i, j) < 0) {
				a += H(i, j);
			}
			else {
				b += H(i, j);
			}
		}
	}
	a *= 255;  // "taken 255"
	b *= 255;

	// https://math.stackexchange.com/questions/914823/shift-numbers-into-a-different-range
	float frac = (d - c) / (b - a);
	Mat_<uchar> normImg(convImg.rows, convImg.cols);
	for (int i = 0; i < convImg.rows; i++) {
		for (int j = 0; j < convImg.cols; j++) {
			normImg(i, j) = c + frac * (convImg(i, j) - a);
		}
	}

	return normImg;
}

// todo - what happens for w % 2 == 0?
Mat_<float> convolution(Mat_<uchar> img, Mat_<float> H)
{
	Mat_<float> convImg(img.rows, img.cols);
	const int halfH = H.rows / 2;
	const int halfW = H.cols / 2;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			convImg(i, j) = 0;
			for (int u = 0; u < H.rows; u++)
			{
				for (int v = 0; v < H.cols; v++)
				{
					int di = i + u - halfH;
					int dj = j + v - halfW;
					if (!isInside(img, di, dj)) {
						continue;
					}
					convImg(i, j) += H(u, v) * img(di, dj);
				}
			}
		}
	}

	return convImg;
}

// todo - optimization 2 convolutions?
Mat_<uchar> gaussianFilter(Mat_<uchar> img, int w)
{
	// build kernel - see IP10
	float sigma = w / 6.0;
	float sigma2 = sigma * sigma;
	float frac = 1 / (2 * PI * sigma2);
	int x0 = w / 2;
	int y0 = w / 2;

	Mat_<float> kernel(w, w);
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < w; y++)
		{
			kernel(y, x) = frac * exp(
				-((x - x0) * (x - x0) + (y - y0) * (y - y0)) /
				(2 * sigma2)
			);
		}
	}

	Mat_<float> filteredImg = convolution(img, kernel);
	return floatToGrayscale(filteredImg, kernel);
}


Mat_<uchar> canny(Mat_<uchar> img)
{
	// get gradients by applying sobel
	float sobelX_[9] = {
		-1,		0,		1,
		-2,		0,		2,
		-1,		0,		1
	};
	float sobelY_[9] = {
		 1,		2,		1,
		 0,		0,		0,
		-1,	   -2,	   -1
	};
	Mat_<float> sobelX = Mat(3, 3, CV_32FC1, sobelX_);
	Mat_<float> sobelY = Mat(3, 3, CV_32FC1, sobelY_);

	Mat_<float> dx = convolution(img, sobelX);
	Mat_<float> dy = convolution(img, sobelY);


	// get magnitude and angle from gradients
	Mat_<float> mag(img.rows, img.cols);
	Mat_<float> ang(img.rows, img.cols);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			mag(i, j) = sqrt(dx(i, j) * dx(i, j) + dy(i, j) * dy(i, j));  // todo - is i and j used well, or inversed?
			ang(i, j) = atan2(dy(i, j), dx(i, j));
		}
	}

	// quantize the angles (from degrees to 0-3)
	Mat_<int> q(img.rows, img.cols);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			float a = ang(i, j);
			if (a < 0) {
				a += 2 * PI;
			}
			q(i, j) = int(round(a / (2 * PI) * 8)) % 8;  // todo - understand
		}
	}

	// 4. Edge thinning
	int di[] = { 1,  1,  0, -1, -1, -1,  0,  1 };
	int dj[] = { 0,  1,  1,  1,  0, -1, -1, -1 };

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			// neighbor in gradient direction
			int iGrad = i + di[q(i, j)];
			int jGrad = j + dj[q(i, j)];
			int grad;

			// neighbor in direction opposite to gradient
			int iGradOpposite = i + di[(q(i, j) + 4) % 8];
			int jGradOpposite = j + dj[(q(i, j) + 4) % 8];
			int gradOpposite;

			// pad with 0
			if (isInside(img, iGrad, jGrad)) {
				grad = mag(iGrad, jGrad);
			}
			else {
				grad = 0;
			}

			// pad with 0
			if (isInside(img, iGradOpposite, jGradOpposite)) {
				gradOpposite = mag(iGradOpposite, jGradOpposite);
			}
			else {
				gradOpposite = 0;
			}

			// if not local maxima, rejected
			if (abs(mag(i, j)) < abs(grad) || abs(mag(i, j)) < abs(gradOpposite)) {
				mag(i, j) = 0;  // TODO copy
			}
		}
	}

	// 5. Thresholding
	float thr1 = 100;
	float thr2 = 200;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (mag(i, j) < thr1) {
				mag(i, j) = 0;		// non-edge
			}
			else if (mag(i, j) < thr2) {
				mag(i, j) = 50;		// weak-edge
			}
			else {
				mag(i, j) = 255;	// strong-edge
			}
		}
	}

	int pi, pj;	// pixel index
	int ni, nj;	// neighbor index (neighbor of pixel)
	Mat_<bool> visited(img.rows, img.cols, false);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			// if pixel not strong-edge or already visited
			if (mag(i, j) != 255 || visited(i, j)) {
				continue;
			}

			// start of a new connected component (new BFS)
			std::queue<std::pair<int, int>> Q;

			// label pixel and enqueue
			visited(i, j) = true;
			Q.push(std::pair<int, int>(i, j));

			while (!Q.empty()) {
				// dequeue and decompose
				std::pair<int, int> p = Q.front();
				pi = p.first; pj = p.second;
				Q.pop();

				// for each neighbor
				for (int k = 0; k < 8; k++) {
					ni = pi + di[k];
					nj = pj + dj[k];

					// if not inside, discard
					if (!isInside(img, ni, nj)) {
						continue;
					}

					// if non-edge or visited, discard
					if (mag(ni, nj) == 0 || visited(ni, nj)) {
						continue;
					}

					// transform weak-edge to strong edge
					if (mag(ni, nj) == 50) {
						mag(ni, nj) = 255;
					}

					visited(ni, nj) = true;
					Q.push(std::pair<int, int>(ni, nj));
				}
			}
		}
	}

	Mat_<uchar> toReturn(mag.rows, mag.cols);
	for (int i = 0; i < mag.rows; i++)
	{
		for (int j = 0; j < mag.cols; j++)
		{
			if (mag(i, j) == 255)
			{
				toReturn(i, j) = 255;
			}
			else
			{
				toReturn(i, j) = 0;
			}
		}
	}
	return toReturn;
}

int main()
{
	Mat_<uchar> imgOriginalGrayscale = imread("Images\\empty_board_1.jpg", IMREAD_GRAYSCALE);
	if (imgOriginalGrayscale.empty())
	{
		std::cout << "Could not open image!" << std::endl;
		return 1;
	}
	Mat_<Vec3b> imgOriginalColor = imread("Images\\empty_board_1.jpg", IMREAD_COLOR);
	if (imgOriginalColor.empty())
	{
		std::cout << "Could not open image!" << std::endl;
		return 1;
	}


	Mat_<uchar> imgResizedGrayscale;
	resize(imgOriginalGrayscale, imgResizedGrayscale, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));
	Mat_<Vec3b> imgResizedColor;
	resize(imgOriginalColor, imgResizedColor, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	cv::imshow("imgGrayscale", imgResizedGrayscale);
	Mat_<uchar> imgBinary = grayscaleToBinary(imgResizedGrayscale, BINARY_THRESHOLD);
	cv::imshow("imgBinary", imgBinary);

	Mat_<uchar> imgGauss = gaussianFilter(imgResizedGrayscale, 7);
	imshow("imgGauss", imgGauss);
	Mat_<uchar> imgBinaryGauss = grayscaleToBinary(imgGauss, BINARY_THRESHOLD);
	cv::imshow("imgBinaryGauss", imgBinaryGauss);

	uchar selPattern[] = {
		0,		0,		0,	0, 0, 0, 0,
		0,		0,		0,	0, 0, 0, 0,
		0,		0,		0,	0, 0, 0, 0,
		0,		0,		0,	0, 0, 0, 0,
		0,		0,		0,	0, 0, 0, 0,
		0,		0,		0,	0, 0, 0, 0,
		0,		0,		0,	0, 0, 0, 0
	};
	const cv::Mat_<uchar> sel = cv::Mat(7, 7, CV_8UC1, selPattern);

	Mat_<uchar> imgClosed = closing(imgBinary, sel);
	Mat_<uchar> imgClosedGauss = closing(imgBinaryGauss, sel);
	imshow("imgClosed", imgClosed);
	imshow("imgClosedGauss", imgClosedGauss);

	Mat_<uchar> imgCanny = canny(imgClosedGauss);
	imshow("imgCanny", imgCanny);

	std::vector<Point2i> points;
	for (int i = 0; i < imgCanny.rows; i++)
	{
		for (int j = 0; j < imgCanny.cols; j++)
		{
			if (imgCanny(i, j) == 255)
			{
				points.push_back(Point2i(j, i));
			}
		}
	}
	Mat_<Vec3b> imgHough = hough(points, imgResizedColor);
	imshow("imgHough", imgHough);

	waitKey();
	/*
	uchar selPattern[] = {
		0,		0,		0,
		0,		0,		0,
		0,		0,		0,
	};
	const cv::Mat_<uchar> sel = cv::Mat(3, 3, CV_8UC1, selPattern);
	Mat_<uchar> imgOpened = closing(imgBinary, sel);


	std::vector<cv::Point2i> points = borderTracing(imgOpened);
	cv::Mat_<cv::Vec3b> imgBorder = binaryToColor(imgOpened);
	for (Point2i point : points)
	{
		imgBorder(point.y, point.x) = cv::Vec3b(0, 0, 255);
	}

	//cv::imshow("imgResized", imgResizedGrayscale);
	//cv::imshow("imgBinary", imgBinary);
	//cv::imshow("imgOpened", imgOpened);
	//cv::imshow("imgBorder", imgBorder);

	std::vector<Point2i> corners = hough(points, imgBorder);
	calculateAverageColor(imgResizedColor, corners);

	waitKey(0);

	// suppose we get 4 corners
	cv::Point2i topRight = cv::Point2i(350, 50);
	cv::Point2i bottomRight = cv::Point2i(470, 300);
	cv::Point2i bottomLeft = cv::Point2i(170, 430);
	cv::Point2i topLeft = cv::Point2i(120, 100);

	for (Point2i point : points)
	{

	}

	Mat_<uchar> imgCorners = imgResized;
	drawCross(imgCorners, topRight, 25, 0);
	drawCross(imgCorners, bottomRight, 50, 0);
	drawCross(imgCorners, bottomLeft, 75, 0);
	drawCross(imgCorners, topLeft, 100, 0);

	imshow("imgCorners", imgCorners);
	moveWindow("imgCorners", 400, 200);
	waitKey();
	*/
	return 0;
}