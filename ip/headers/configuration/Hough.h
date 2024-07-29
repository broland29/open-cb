#pragma once

#include "../CommonIP.h"


struct Peak
{
	int theta, rho, hval;
	bool operator < (const Peak& o) const
	{
		return hval > o.hval;
	}
};


// "The normal parameterization of a line consists of representing the line
// by its normal vector and the distance from origin to the line."
struct lineRhoTheta
{
	int rho;    // distance in pixels [0, DiameterOfImage]
	int theta;  // angle in grades [0, 360)

	// draw the line on img and return result
	Mat_<Vec3b> drawLine(Mat_<Vec3b> img);

	int getIntersection(lineRhoTheta otherLine, Point2i& intersectionPoint);
};


std::vector<lineRhoTheta> hough(
	std::vector<Point2i> points,	// object pixels of border image
	int imgRows,					// rows of border image
	int imgCols,					// columns of border image
	int rhoStepSize,				// increment for rho at one step
	int thetaStepSize,				// increment for theta at one step
	int windowSize,					// size of window from which we take local maxima
	int noOfLines					// number of lines we wish to get. if less found, less returned
);
