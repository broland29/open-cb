#include "../../headers/Util.h"
#include "../../headers/crop/Conversion.h"


Mat_<uchar> grayscaleToBinary(Mat_<uchar> img, int threshold)
{
	Mat_<uchar> imgRes(img.rows, img.cols);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img(i, j) < threshold) {
				imgRes(i, j) = 0;		// black
			}
			else {
				imgRes(i, j) = 255;		// white
			}
		}
	}

	return imgRes;
}


Mat_<Vec3b> binaryToColor(Mat_<uchar> img)
{
	Mat_<Vec3b> imgRes(img.rows, img.cols);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img(i, j) == 0) {
				imgRes(i, j) = Vec3b(0, 0, 0);			// black
			}
			else {
				imgRes(i, j) = Vec3b(255, 255, 255);	// white
			}
		}
	}

	return imgRes;
}


// normalization
// convImg - the result of the convolution
// H - the kernel used for the convolution of convImg
Mat_<uchar> floatToGrayscale(Mat_<float> convImg, Mat_<float> H)
{
	// map [a, b] to [c, d]
	float a = 0;		// min conv value we can get -> every positive taken 0 and every negative taken 255 (since original image grayscale)
	float b = 0;		// max conv value we can get -> opposite approach
	float c = 0;		// min grayscale
	float d = 255;		// max grayscale

	for (int i = 0; i < H.rows; i++) {
		for (int j = 0; j < H.cols; j++) {
			if (H(i, j) < 0) {
				a += H(i, j);
			}
			else {
				b += H(i, j);
			}
		}
	}
	a *= 255;  // "taken 255"
	b *= 255;

	// https://math.stackexchange.com/questions/914823/shift-numbers-into-a-different-range
	float frac = (d - c) / (b - a);
	Mat_<uchar> normImg(convImg.rows, convImg.cols);
	for (int i = 0; i < convImg.rows; i++) {
		for (int j = 0; j < convImg.cols; j++) {
			normImg(i, j) = c + frac * (convImg(i, j) - a);
		}
	}

	return normImg;
}