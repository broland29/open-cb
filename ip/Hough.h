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
Mat_<Vec3b> hough(
    std::vector<Point2i> points,
    cv::Mat_<cv::Vec3b> imgBorder
);