#pragma once

#include "../Common.h"
#include "Filter.h"


// Performs Canny Edge Detection on img. Returns a binary image.
Mat_<uchar> canny(
    Mat_<uchar> img
);
