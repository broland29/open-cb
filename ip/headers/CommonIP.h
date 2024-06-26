#pragma once

#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

using namespace cv;

const double PI = 3.14159265;

const int IMAGE_WIDTH = 500;
const int IMAGE_HEIGHT = 500;
const int IMAGE_EDGE_SIZE = 3;


int inline cellWidth(int borderRight, int borderLeft)
{
    return (IMAGE_WIDTH - borderRight - borderLeft) / 8;
}

int inline cellHeight(int borderTop, int borderBottom)
{
    return (IMAGE_HEIGHT - borderTop - borderBottom) / 8;
}


// Checks if pixel (i,j) is inside img's boundaries
bool inline isInside(Mat img, int i, int j)
{
    return (i >= 0 && i < img.rows) && (j >= 0 && j < img.cols);
}

// Checks if pixel is on the edge of the image (does not imply isInside)
bool inline isImageEdgePixel(Mat img, int i, int j)
{
    return i < IMAGE_EDGE_SIZE || i >= img.rows - IMAGE_EDGE_SIZE || j < IMAGE_EDGE_SIZE || j >= img.cols - IMAGE_EDGE_SIZE;
}
