#include "../../headers/configuration/Filter.h"


Mat_<uchar> gaussianFilter(Mat_<uchar> img, int w)
{
	// build kernel - see IP10
	double sigma = w / 6.0;
	double sigma2 = sigma * sigma;
	double frac = 1 / (2 * PI * sigma2);
	int x0 = w / 2;
	int y0 = w / 2;

	Mat_<double> kernel(w, w);
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