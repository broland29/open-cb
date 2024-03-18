#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>

using namespace cv;

#define PI 3.14159265

#define IMAGE_WIDTH				500
#define IMAGE_HEIGHT			500
#define BORDER_SIZE             25      // 500 / 8 = 62.5 one cell (2.5 cm) -> border (1 cm) = 62.5 / 2.5 = 25
#define CELL_WIDTH              (IMAGE_WIDTH - 2 * BORDER_SIZE) / 8
#define CELL_HEIGHT             (IMAGE_HEIGHT - 2 * BORDER_SIZE) / 8

#define PATH_IMG_CAM_ONE        "C:\\open-cb\\mem\\cam1.jpeg"
#define PATH_IMG_CAM_TWO        "C:\\open-cb\\mem\\cam2.jpeg"
#define PATH_DUMMY_IMG_CAM_ONE  "C:\\open-cb\\ua\\preview\\dummy_cam_left.jpeg"
#define PATH_DUMMY_IMG_CAM_TWO  "C:\\open-cb\\ua\\preview\\dummy_cam_right.jpeg"

#define TRAIN_FOLDER_PATH "C:\\open-cb\\mem\\train"
#define TEST_FOLDER_PATH "C:\\open-cb\\mem\\test"


#define CLASS_COUNT 14

// "Standard behavior: Windows file system treats file and directory names as case-insensitive"
//  so, unfortunately, cannot use P and p, must use WP and BP
static inline std::string folders[] = { "WF", "WP", "WB", "WN", "WR", "WQ", "WK", "BF", "BP", "BB", "BN", "BR", "BQ", "BK" };


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


// extracts cell of interest and cell above as well
// fails for row 0, since out of bounds. but it should not be used for row 0! since only lower part of interest
Mat_<Vec3b> extractCell(int i, int j, Mat_<Vec3b> img);


void mapCharToFolder(char enc, char folder[3]);


