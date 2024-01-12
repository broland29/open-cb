#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>

using namespace cv;

#define PI 3.14159265

// "The normal parameterization of a line consists of representing the line
// by its normal vector and the distance from origin to the line."
struct lineRoTheta
{
    int ro;     // distance in pixels [0, DiameterOfImage]
    int theta;  // angle in grades [0, 360]

    Mat_<Vec3b> drawLine(Mat_<Vec3b> img);

    int getIntersection(lineRoTheta otherLine, Point2i &intersectionPoint);
};


// Checks if pixel (i,j) is inside img's boundaries
bool isInside(
    Mat img,
    int i,
    int j
);


// Draws a cross on grayscale image img, "around" point p, with given diameter
// and optionally color (white default)
void drawCrossGrayscale(
    Mat_<uchar> img,
    Point2i p,
    int diameter,
    int color = 255
);


// Draws a cross on color image img, "around" point p, with given diameter
// and optionally color (white default)
void drawCrossColor(
    Mat_<Vec3b> img,
    Point2i p,
    int diameter,
    Vec3b color = Vec3b(255, 255, 255)
);


// Performs convolution on img with kernel H
Mat_<float> convolution(
    Mat_<uchar> img,
    Mat_<float> H,
    uchar padding = 255
);


// Extract object pixels in a list of points
std::vector<Point2i> getPointsFromBinary(
    Mat_<uchar> img,
    int objectPixelColor = 255
);


// Gets the intersection of two lines defined by a pair of points
int getIntersectionOfLines(
    std::pair<Point2i, Point2i> lineAB,
    std::pair<Point2i, Point2i> lineCD,
    Mat img,
    Point2i &intersection
);

double euclideanDistance(Point2i a, Point2i b);
