#include "../../headers/Util.h"
#include "../../headers/crop/Filter.h"
#include "../../headers/crop/Conversion.h"


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