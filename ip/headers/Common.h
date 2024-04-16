#pragma once

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

using namespace cv;

#define PI 3.14159265

#define IMAGE_WIDTH			500
#define IMAGE_HEIGHT		500
#define BORDER_LEFT         53
#define BORDER_RIGHT        50
#define BORDER_TOP          55
#define BORDER_BOTTOM       70
#define CELL_WIDTH          (IMAGE_WIDTH - BORDER_RIGHT - BORDER_LEFT) / 8
#define CELL_HEIGHT         (IMAGE_HEIGHT - BORDER_TOP - BORDER_BOTTOM) / 8


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

#define CNN_FOLDER_PATH             "C:\\open-cb\\mem\\cnn"

#define CLASS_COUNT 14

// "Standard behavior: Windows file system treats file and directory names as case-insensitive"
//  so, unfortunately, cannot use P and p, must use WP and BP
static inline std::string labelFolders[] =
{
    "WF", "WP", "WB", "WN", "WR", "WQ", "WK", "BF", "BP", "BB", "BN", "BR", "BQ", "BK"
};


// Checks if pixel (i,j) is inside img's boundaries
bool inline isInside(Mat img, int i, int j)
{
    return (i >= 0 && i < img.rows) && (j >= 0 && j < img.cols);
}



