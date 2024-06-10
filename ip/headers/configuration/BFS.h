#pragma once

#include "../CommonIP.h"


// starts BFS from every black pixel close to the edge of the image and makes the
// black pixels met white (it is presumed that the background is supposed to be white
// and the board is supposed to be black)
Mat_<uchar> fillBFS(Mat_<uchar> img);
