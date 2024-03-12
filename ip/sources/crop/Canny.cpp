#include "../../headers/Util.h"
#include "../../headers/crop/Canny.h"


Mat_<uchar> canny(Mat_<uchar> img)
{
	// get gradients by applying sobel
	float sobelX_[9] =
	{
		-1,		0,		1,
		-2,		0,		2,
		-1,		0,		1
	};
	float sobelY_[9] =
	{
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
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			mag(i, j) = sqrt(dx(i, j) * dx(i, j) + dy(i, j) * dy(i, j));  // todo - is i and j used well, or inversed?
			ang(i, j) = atan2(dy(i, j), dx(i, j));
		}
	}

	// quantize the angles (from degrees to 0-3)
	Mat_<int> q(img.rows, img.cols);
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			float a = ang(i, j);
			if (a < 0)
			{
				a += 2 * PI;
			}
			q(i, j) = int(round(a / (2 * PI) * 8)) % 8;  // todo - understand
		}
	}

	// 4. Edge thinning
	int di[] = { 1,  1,  0, -1, -1, -1,  0,  1 };
	int dj[] = { 0,  1,  1,  1,  0, -1, -1, -1 };

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			// neighbor in gradient direction
			int iGrad = i + di[q(i, j)];
			int jGrad = j + dj[q(i, j)];
			int grad;

			// neighbor in direction opposite to gradient
			int iGradOpposite = i + di[(q(i, j) + 4) % 8];
			int jGradOpposite = j + dj[(q(i, j) + 4) % 8];
			int gradOpposite;

			// pad with 0
			if (isInside(img, iGrad, jGrad))
			{
				grad = mag(iGrad, jGrad);
			}
			else
			{
				grad = 0;
			}

			// pad with 0
			if (isInside(img, iGradOpposite, jGradOpposite))
			{
				gradOpposite = mag(iGradOpposite, jGradOpposite);
			}
			else
			{
				gradOpposite = 0;
			}

			// if not local maxima, rejected
			if (abs(mag(i, j)) < abs(grad) || abs(mag(i, j)) < abs(gradOpposite))
			{
				mag(i, j) = 0;  // TODO copy
			}
		}
	}

	// 5. Thresholding
	float thr1 = 100;
	float thr2 = 200;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (mag(i, j) < thr1)
			{
				mag(i, j) = 0;		// non-edge
			}
			else if (mag(i, j) < thr2)
			{
				mag(i, j) = 50;		// weak-edge
			}
			else
			{
				mag(i, j) = 255;	// strong-edge
			}
		}
	}

	int pi, pj;	// pixel index
	int ni, nj;	// neighbor index (neighbor of pixel)
	Mat_<bool> visited(img.rows, img.cols, false);
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			// if pixel not strong-edge or already visited
			if (mag(i, j) != 255 || visited(i, j))
			{
				continue;
			}

			// start of a new connected component (new BFS)
			std::queue<std::pair<int, int>> Q;

			// label pixel and enqueue
			visited(i, j) = true;
			Q.push(std::pair<int, int>(i, j));

			while (!Q.empty())
			{
				// dequeue and decompose
				std::pair<int, int> p = Q.front();
				pi = p.first; pj = p.second;
				Q.pop();

				// for each neighbor
				for (int k = 0; k < 8; k++)
				{
					ni = pi + di[k];
					nj = pj + dj[k];

					// if not inside, discard
					if (!isInside(img, ni, nj))
					{
						continue;
					}

					// if non-edge or visited, discard
					if (mag(ni, nj) == 0 || visited(ni, nj))
					{
						continue;
					}

					// transform weak-edge to strong edge
					if (mag(ni, nj) == 50)
					{
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