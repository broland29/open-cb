#include "../../headers/configuration/BFS.h"

#define IMAGE_EDGE_SIZE 3


bool inline isImageEdgePixel(Mat img, int i, int j)
{
	return i < IMAGE_EDGE_SIZE || i >= img.rows - IMAGE_EDGE_SIZE || j < IMAGE_EDGE_SIZE || j >= img.cols - IMAGE_EDGE_SIZE;
}


Mat_<uchar> fillBFS(Mat_<uchar> img)
{
	Mat_<uchar> imgPreprocessed = img.clone();

	// index offsets for 8-neighborhood
	int di[8] = { -1, -1, -1, 0, 1, 1,  1,  0 };
	int dj[8] = { -1,  0,  1, 1, 1, 0, -1, -1 };

	for (int i = 0; i < imgPreprocessed.rows; i++)
	{
		for (int j = 0; j < imgPreprocessed.cols; j++)
		{
			// when starting BFS, only consider in black pixels on the edge 
			if (imgPreprocessed(i, j) != 0)
			{
				continue;
			}
			if (!isImageEdgePixel(imgPreprocessed, i, j))
			{
				continue;
			}

			// start a new BFS
			std::queue<std::pair<int, int>> Q;
			Q.push(std::pair<int, int>(i, j));
			while (!Q.empty())
			{
				int ci = Q.front().first;
				int cj = Q.front().second;
				Q.pop();

				// for each neighbor
				for (int n = 0; n < 8; n++)
				{
					int ni = ci + di[n];
					int nj = cj + dj[n];

					// neighbors can go out of bounds
					if (!isInside(imgPreprocessed, ni, nj))
					{
						continue;
					}

					// when "spanning BFS", we go out of the edge as well, as long as we meet black pixels 
					if (imgPreprocessed(ni, nj) != 0)
					{
						continue;
					}

					imgPreprocessed(ni, nj) = 255;  // no need to keep track of visited pixels, since white discarded anyways
					Q.push(std::pair<int, int>(ni, nj));
				}
			}
		}
	}

	return imgPreprocessed;
}