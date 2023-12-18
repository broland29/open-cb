#pragma once


// Performs border tracing on a binary image, starting from first object pixel.
// Returns a vector of points of the border.
std::vector<Point2i> borderTracing(
    Mat_<uchar> img
);
