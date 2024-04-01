#pragma once

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

using namespace cv;

#define PI 3.14159265

#define IMAGE_WIDTH			500
#define IMAGE_HEIGHT		500
#define BORDER_SIZE         25      // 500 / 8 = 62.5 one cell (2.5 cm) -> border (1 cm) = 62.5 / 2.5 = 25
#define CELL_WIDTH          (IMAGE_WIDTH - 2 * BORDER_SIZE) / 8
#define CELL_HEIGHT         (IMAGE_HEIGHT - 2 * BORDER_SIZE) / 8


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


std::string inline cellImageName(int row, int col)
{
    return std::string("board") + std::to_string(row) + std::to_string(col) + std::string(".jpeg");
}