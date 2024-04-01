#pragma once

#include "../Common.h"


struct Peak
{
	int theta, ro, hval;
	bool operator < (const Peak& o) const
	{
		return hval > o.hval;
	}
};


// "The normal parameterization of a line consists of representing the line
// by its normal vector and the distance from origin to the line."
struct lineRoTheta
{
	int ro;     // distance in pixels [0, DiameterOfImage]
	int theta;  // angle in grades [0, 360]

	Mat_<Vec3b> drawLine(Mat_<Vec3b> img);

	int getIntersection(lineRoTheta otherLine, Point2i& intersectionPoint);
};


// performs Hough, returns line image?
std::vector<lineRoTheta> hough(
	std::vector<Point2i> points,
	cv::Mat_<cv::Vec3b> imgBorder,
	int roStepSize,
	int thetaStepSize,
	int windowSize,
	int noOfLines
);
