#include "stdafx.h"
#include "Util.h"
#include "Morphological.h"


Mat_<uchar> erosion(Mat_<uchar> img, Mat_<uchar> sel)
{
	Mat_<uchar> imgRes(img.rows, img.cols, 255);

	// iterate original pixels
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			// only consider object pixels from original
			if (img(i, j) != 0) {
				continue;
			}

			// iterate pixels given by structuring element
			//		"If the structuring element covers any background pixel,
			//		 the pixel in the result image keeps its background label."
			for (int u = 0; u < sel.rows; u++)
			{
				for (int v = 0; v < sel.cols; v++)
				{
					// only consider object pixels from structuring element
					if (sel(u, v) != 0)
					{
						continue;
					}

					// offset structuring element's pixel
					int i2 = u - sel.rows / 2 + i;
					int j2 = v - sel.cols / 2 + j;

					if (isInside(imgRes, i2, j2) && img(i2, j2) != 0)
					{
						goto skip;
					}
				}
			}
			imgRes(i, j) = 0;
		skip:;
		}
	}

	return imgRes;
}

// Perform dilation on img, with structuring element sel
Mat_<uchar> dilation(Mat_<uchar> img, Mat_<uchar> sel)
{
	Mat_<uchar> imgRes(img.rows, img.cols, 255);

	// iterate original pixels
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			// only consider object pixels from original
			if (img(i, j) != 0)
			{
				continue;
			}

			// iterate pixels given by structuring element
			//		"If the origin of the structuring element coincides with an object pixel in the image,
			//		 label all pixels covered by the structuring element as object pixels in the result image."
			for (int u = 0; u < sel.rows; u++)
			{
				for (int v = 0; v < sel.cols; v++)
				{
					// only consider object pixels from structuring element
					if (sel(u, v) != 0)
					{
						continue;
					}

					// offset structuring element's pixel
					int i2 = u - sel.rows / 2 + i;
					int j2 = v - sel.cols / 2 + j;

					if (isInside(imgRes, i2, j2))
					{
						imgRes(i2, j2) = 0;
					}
				}
			}
		}
	}

	return imgRes;
}


Mat_<uchar> opening(Mat_<uchar> img, Mat_<uchar> sel)
{
	return dilation(erosion(img, sel), sel);
}


Mat_<uchar> closing(Mat_<uchar> img, Mat_<uchar> sel)
{
	return erosion(dilation(img, sel), sel);
}