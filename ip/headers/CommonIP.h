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

int inline cellWidth(int borderRight, int borderLeft)
{
    return (IMAGE_WIDTH - borderRight - borderLeft) / 8;
}

int inline cellHeight(int borderTop, int borderBottom)
{
    return (IMAGE_HEIGHT - borderTop - borderBottom) / 8;
}


#define PREVIEW_LEFT_PATH       "C:\\open-cb\\mem\\img\\pre\\cam_left.jpeg"
#define PREVIEW_RIGHT_PATH      "C:\\open-cb\\mem\\img\\pre\\cam_right.jpeg"


// "One should augment the data after Train and Test split. To work correctly 
//    one needs to make sure to augment data only from the train split."
// label folder base paths
#define TEMPORARY_FOLDER_PATH       "C:\\open-cb\\mem\\img\\tmp"  // images labeled but yet put in trn/val/tst set
#define TRAIN_FOLDER_PATH           "C:\\open-cb\\mem\\img\\trn"  // images for training
#define VALIDATION_FOLDER_PATH      "C:\\open-cb\\mem\\img\\val"  // images for validation
#define TEST_FOLDER_PATH            "C:\\open-cb\\mem\\img\\tst"  // images for testing
#define BOARD_FOLDER_PATH           "C:\\open-cb\\mem\\img\\brd"  // images for actual classification (cells from one image)
#define GRAB_FOLDER_PATH             "C:\\open-cb\\mem\\img\\grb" // images grabbed by CameraReader, but not yet used

#define KNN_FOLDER_PATH             "C:\\open-cb\\mem\\knn"
#define CNN_FOLDER_PATH             "C:\\open-cb\\mem\\cnn"


#define JSON_PATH                   "C:\\open-cb\\mem\\per\\persistence.json"              


// Checks if pixel (i,j) is inside img's boundaries
bool inline isInside(Mat img, int i, int j)
{
    return (i >= 0 && i < img.rows) && (j >= 0 && j < img.cols);
}




