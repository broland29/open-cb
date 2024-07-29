#include "../../headers/configuration/Hough.h"


std::vector<lineRhoTheta> hough(std::vector<Point2i> points, int imgRows, int imgCols, int rhoStepSize, int thetaStepSize, int windowSize, int noOfLines)
{
	const int roMax = sqrt(imgRows * imgRows + imgCols + imgCols);
	const int thetaMax = 359;

	Mat_<int> Hough(roMax / rhoStepSize + 1, thetaMax / thetaStepSize + 1);  // Hough accumulator (type int, since char may overflow)
	Hough.setTo(0);

	// fill accumulator: for each point, for each (appropriate) angle, for each (valid) rho cast a vote.
	for (Point2i point : points)
	{
		for (int thetaAngle = 0; thetaAngle <= thetaMax; thetaAngle += thetaStepSize)
		{
			// skip for thetas which give lines out of image
			if (thetaAngle > 180 && thetaAngle < 270)
			{
				continue;
			}

			float thetaRadian = thetaAngle * PI / 180;  // sin and cos expect radians; float!
			int ro = point.x * cos(thetaRadian) + point.y * sin(thetaRadian);  // from parametric equation
			
			if (ro >= 0 && ro <= roMax)  // in valid range
			{
				Hough(ro / rhoStepSize, thetaAngle / thetaStepSize)++;  // thetaAngle!
			}
		}
	}

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

					// skip invalid rho
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
				peaks.push_back(Peak{ j * thetaStepSize, i * rhoStepSize, localMax });  // need to "wrap back" to original values
			}
		}
	}

	std::sort(peaks.begin(), peaks.end());

	if (noOfLines > peaks.size())
	{
		SPDLOG_INFO("Could not find {} lines, only {}", noOfLines, peaks.size());
		noOfLines = peaks.size();
	}

	// wrap result
	std::vector<lineRhoTheta> lines;
	for (int i = 0; i < noOfLines; i++)
	{
		lines.push_back(lineRhoTheta{ peaks[i].rho, peaks[i].theta });
	}

	return lines;
}


Mat_<Vec3b> lineRhoTheta::drawLine(Mat_<Vec3b> img)
{
	Mat_<Vec3b> linesImg = img.clone();
	float thetaRadian = theta * PI / 180;  // sin, cos expects radians

	Point2i A, B;
	if (thetaRadian > -(PI / 4) && thetaRadian < PI / 4)  // avoids division by 0
	{
		A = Point2i(0, rho / sin(thetaRadian));  // take x = 0
		B = Point2i(linesImg.cols, (rho - linesImg.cols * cos(thetaRadian)) / sin(thetaRadian));
	}
	else
	{
		A = Point2i(rho / cos(thetaRadian), 0);
		B = Point2i((rho - linesImg.rows * sin(thetaRadian)) / cos(thetaRadian), linesImg.rows);
	}
	line(linesImg, A, B, Scalar(0, 255, 0), 3);

	return linesImg;
}


int lineRhoTheta::getIntersection(lineRhoTheta otherLine, Point2i& intersectionPoint)
{
	// https://stackoverflow.com/questions/383480/intersection-of-two-lines-defined-in-rho-theta-parameterization
	float r1 = rho;
	float t1 = theta * PI / 180;  // sin, cos expects radians
	float r2 = otherLine.rho;
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
		(sint2 * r1 - sint1 * r2) / det,
		(-cost2 * r1 + cost1 * r2) / det
	);
	return 0;
}