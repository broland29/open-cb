#include "stdafx.h"
#include "Util.h"
#include "Hough.h"


std::vector<std::pair<Point2i, Point2i>> hough(
	std::vector<Point2i> points,
	cv::Mat_<cv::Vec3b> imgBorder,
	int roStepSize,
	int thetaStepSize,
	int windowSize,
	int noOfLines
)
{
	int D = sqrt(imgBorder.rows * imgBorder.rows + imgBorder.cols + imgBorder.cols);	// diagonal, maximum value of ro
	const int roMax = D;
	const int thetaMax = 360;
	Mat_<int> Hough(roMax / roStepSize + 1, thetaMax / thetaStepSize + 1, CV_32SC1);												// Hough accumulator (int since char may overflow)
	Hough.setTo(0);

	for (Point2i point : points)
	{
		for (int thetaAngle = 0; thetaAngle < thetaMax; thetaAngle += thetaStepSize)
		{
			// skip for thetas which give lines out of image
			if (thetaAngle >= 180 && thetaAngle <= 270)
			{
				continue;
			}

			float thetaRadian = thetaAngle * PI / 180;  // sin and cos expect radians; float!
			int ro = point.x * cos(thetaRadian) + point.y * sin(thetaRadian);
			if (ro >= 0 && ro <= D)
			{
				Hough(ro / roStepSize, thetaAngle / thetaStepSize)++;  // thetaAngle!
			}
		}
	}

	/*
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
	*/

	std::vector<Peak> peaks;
	for (int i = 0; i < Hough.rows; i++)
	{
		for (int j = 0; j < Hough.cols; j++)
		{
			int localMax = 0;
			for (int u = -windowSize / 2; u <= windowSize / 2; u++)
			{
				for (int v = -windowSize / 2; v <= windowSize / 2; v++)
				{
					int ci = i + u;
					int cj = j + v;

					// skip invalid ro
					if (ci < 0 || ci >= Hough.rows)
					{
						continue;
					}

					// to wrap around invalid theta
					cj = (cj + Hough.cols) % Hough.cols;

					if (Hough(ci, cj) > localMax)
					{
						localMax = Hough(ci, cj);
					}
				}
			}
			if (Hough(i, j) == localMax)
			{
				peaks.push_back(Peak{ j * thetaStepSize, i * roStepSize, localMax });  // need to "wrap back" to original values
			}
		}
	}

	std::sort(peaks.begin(), peaks.end());

	Mat_<Vec3b> linesImg = imgBorder;
	if (noOfLines > peaks.size())
	{
		noOfLines = peaks.size();
		std::cout << "Number of lines cut down to " << noOfLines << " !" << std::endl;
	}

	std::vector<std::pair<Point2i, Point2i>> lines;
	for (int i = 0; i < noOfLines; i++)
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
		lines.push_back(std::pair<Point2i, Point2i>(A, B));
		// std::cout << A << " " << B << std::endl;
	}
	imshow("linesImg", linesImg);

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
	return lines;
}