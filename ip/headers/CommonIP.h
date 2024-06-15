#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <opencv2/opencv.hpp>
#include <QString>
#include <QVector>

using namespace cv;

#define PI 3.14159265

#define IMAGE_WIDTH			500
#define IMAGE_HEIGHT		500
#define IMAGE_EDGE_SIZE 3


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
