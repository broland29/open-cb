#include "../headers/Util.h"
#include "../headers/BorderTracing.h"


std::vector<Point2i> borderTracing(Mat_<uchar> img)
{
	std::vector<Point2i> points;	// points of border

	// search for first pixel
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img(i, j) == 0)
			{
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
		if (prevDir % 2 == 0)
		{
			currDir = (prevDir + 7) % 8;
		}
		else
		{
			currDir = (prevDir + 6) % 8;
		}

		// search for neighbor object pixel from beginning direction in counter clockwise direction
		for (int i = 0; i < 8; i++)
		{
			ci = pi + di[currDir];
			cj = pj + dj[currDir];

			if (img(ci, cj) == 0)
			{
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