#include "stdafx.h"
#include "Util.h"


Mat_<Vec3b> lineRoTheta::drawLine(Mat_<Vec3b> img)
{
	Mat_<Vec3b> linesImg = img.clone();
	float thetaRadian = theta * PI / 180;  // sin, cos expects radians

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

	return linesImg;
}


int lineRoTheta::getIntersection(lineRoTheta otherLine, Point2i &intersectionPoint)
{
	// https://stackoverflow.com/questions/383480/intersection-of-two-lines-defined-in-rho-theta-parameterization
	float r1 = ro;
	float t1 = theta * PI / 180;  // sin, cos expects radians
	float r2 = otherLine.ro;
	float t2 = otherLine.theta * PI / 180;

	float cost1 = cos(t1);
	float sint1 = sin(t1);
	float cost2 = cos(t2);
	float sint2 = sin(t2);
	float det = cost1 * sint2 - sint1 * cost2;
	if (det == 0)
	{
		return 1;
	}

	intersectionPoint = Point2i(
		( sint2 * r1 - sint1 * r2) / det,
		(-cost2 * r1 + cost1 * r2) / det
	);
	return 0;
}


bool isInside(Mat img, int i, int j)
{
	return (i >= 0 && i < img.rows) && (j >= 0 && j < img.cols);
}


void drawCrossGrayscale(Mat_<uchar> img, Point2i p, int diameter, int color)
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


void drawCrossColor(Mat_<Vec3b> img, Point2i p, int diameter, Vec3b color)
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


Mat_<float> convolution(Mat_<uchar> img, Mat_<float> H, uchar padding)
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
					if (!isInside(img, di, dj))
					{
						convImg(i, j) += H(u, v) * padding;
						continue;
					}
					convImg(i, j) += H(u, v) * img(di, dj);
				}
			}
		}
	}

	return convImg;
}


std::vector<Point2i> getPointsFromBinary(Mat_<uchar> img, int objectPixelColor)
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


// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line
int getIntersectionOfLines(std::pair<Point2i, Point2i> lineAB, std::pair<Point2i, Point2i> lineCD, Mat img, Point2i &intersection)
{
	int x1 = lineAB.first.x;
	int y1 = lineAB.first.y;
	int x2 = lineAB.second.x;
	int y2 = lineAB.second.y;
	int x3 = lineCD.first.x;
	int y3 = lineCD.first.y;
	int x4 = lineCD.second.x;
	int y4 = lineCD.second.y;

	int denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (denom == 0)
	{
		std::cout << "Lines [" << lineAB.first << "," << lineAB.second;
		std::cout << "] and [" << lineCD.first << "," << lineCD.second;
		std::cout << "] are parallel!" << std::endl;
		return 1;
	}

	intersection = Point2i(
		((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denom,
		((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denom
	);

	if (!isInside(img, intersection.y, intersection.x))
	{
		std::cout << "Lines [" << lineAB.first << "," << lineAB.second;
		std::cout << "] and [" << lineCD.first << "," << lineCD.second;
		std::cout << "] intersect outside of image!" << std::endl;
		return 2;
	}

	return 0;
}
