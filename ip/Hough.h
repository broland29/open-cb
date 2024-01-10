#pragma once


struct Peak
{
	int theta, ro, hval;
	bool operator < (const Peak& o) const
	{
		return hval > o.hval;
	}
};


// performs Hough, returns line image?
std::vector<std::pair<Point2i, Point2i>> hough(
	std::vector<Point2i> points,
	cv::Mat_<cv::Vec3b> imgBorder,
	int roStepSize,
	int thetaStepSize,
	int windowSize,
	int noOfLines
);