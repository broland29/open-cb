#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Client.h"
#include "Util.h"
#include "KNearestNeighbors.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55555

// functions to translate coordinate system from left/right camera to main
// ex: lr - left camera row, mc - main camera column
// see cams.drawio
int leftToMain(int lr, int lc, int &mr, int &mc)
{
	mr = lc;
	mc = 7 - lr;
	return 1;
}

int rightToMain(int rr, int rc, int& mr, int& mc)
{
	mr = 7 - rc;
	mc = rr;
	return 1;
}


int client_main()
{
	// initialize WSA
	WSADATA wsaData;  // structure which will be populated by WSAStartup()
	WORD wVersionRequested = MAKEWORD(2, 2);  // we will use version 2.2
	int wsaErr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaErr != 0)
	{
		std::cout << "WSAStartup() error, code: " << wsaErr << std::endl;
		return 1;
	}
	std::cout << "WSAStartup() success, status: " << wsaData.szSystemStatus << std::endl;

	// create socket
	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "socket() error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	std::cout << "socket() success." << std::endl;

	// connect to server
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(SERVER_PORT);  // server port
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		std::cout << "connect() error: " << WSAGetLastError();
		WSACleanup();
		return 1;
	}
	std::cout << "connect() success." << std::endl;

	char cmd = 'x';
	int currentFileNo = 0;
	while (true)
	{
		// recieve message
		char recvBuffer[200];
		int recvByteCount = recv(clientSocket, recvBuffer, 200, 0);
		if (recvByteCount > 0)
		{
			std::cout << "recv() recieved: " << recvBuffer << std::endl;
		}
		else
		{
			std::cout << "recv() error: " << WSAGetLastError();
			WSACleanup();
		}

		cmd = recvBuffer[0];
		if (cmd == 'e')  // exit
		{
			std::cout << "got cmd = exit" << std::endl;
			return 0;
		}
		if (cmd == 'i' || cmd == 'I')  // request image
		{
			std::cout << "got cmd = request image" << std::endl;

			char sendBuffer[200] = "s";

			cv::VideoCapture cameraOne(0);
			cv::VideoCapture cameraTwo(2);

			if (!cameraOne.isOpened() || !cameraTwo.isOpened())
			{
				if (!cameraOne.isOpened())
				{
					std::cout << "Could not open camera one!" << std::endl;
				}
				if (!cameraTwo.isOpened())
				{
					std::cout << "Could not open camera two!" << std::endl;
				}
				sendBuffer[0] = 'e';

				// notify UA about error
				sendBuffer[0] = 'e';
				int sendByteCount = send(clientSocket, sendBuffer, 200, 0);
				if (sendByteCount > 0)
				{
					std::cout << "send() success." << std::endl;
				}
				else
				{
					std::cout << "send() error: " << WSAGetLastError();
					WSACleanup();
				}
				continue;
			}

			Mat_<Vec3b> imgOriginalColorOne, imgOriginalColorTwo;

			cameraOne >> imgOriginalColorOne;
			cameraTwo >> imgOriginalColorTwo;

			Mat_<Vec3b> imgResizedColorOne, imgResizedColorTwo;
			resize(imgOriginalColorOne, imgResizedColorOne, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));
			resize(imgOriginalColorTwo, imgResizedColorTwo, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));


			cv::imwrite(PATH_IMG_CAM_ONE, imgResizedColorOne);
			cv::imwrite(PATH_IMG_CAM_TWO, imgResizedColorTwo);

			cameraOne.release();
			cameraTwo.release();

			if (cmd == 'I')  // also classify
			{
				// get train images (similar to train)
				const int C = 13;
				const std::array<std::string, C> classes = {
					"black_bishop",
					"black_king",
					"black_knight",
					"black_pawn",
					"black_queen",
					"black_rook",
					"free",
					"white_bishop",
					"white_king",
					"white_knight",
					"white_pawn",
					"white_queen",
					"white_rook"
				};

				std::map<std::string, std::vector<Mat_<Vec3b>>> trainImages;

				for (int i = 0; i < C; i++)
				{
					trainImages.insert(
						std::pair<std::string, std::vector<Mat_<Vec3b>>>(
							classes[i],
							std::vector<Mat_<Vec3b>>()
							));
				}

				char folders[13][3] = { "FR", "WP", "WB", "WN", "WR", "WQ", "WK", "BP", "BB", "BN", "BR", "BQ", "BK" };
				char path[256];
				for (int i = 0; i < 13; i++)
				{
					sprintf(path, "persistence\\train\\%s", folders[i]);
					for (const auto& entry : fs::directory_iterator(path))
					{
						std::string pathString{ entry.path().string() };
						std::cout << pathString << std::endl;

						Mat_<Vec3b> img = imread(pathString, IMREAD_COLOR);
						if (img.empty())
						{
							std::cout << "Could not open file at path: " << pathString << std::endl;
							break;
						}

						trainImages[classes[i]].push_back(img);
					}
				}

				// read images and extract cells (similar to send to train/test)
				Mat_<Vec3b> imgCamLeft = imread(PATH_IMG_CAM_ONE, IMREAD_COLOR);
				Mat_<Vec3b> imgCamRight = imread(PATH_IMG_CAM_TWO, IMREAD_COLOR);
				std::array<std::array<Mat_<Vec3b>, 8>, 8> cells;

				for (int i = 4; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						// coordinate conversion for naming
						// i, j (coord system of camera x) -> xRow, xCol (main coord system)
						int leftRow, leftCol, rightRow, rightCol;
						leftToMain(i, j, leftRow, leftCol);
						rightToMain(i, j, rightRow, rightCol);

						// extract and save cell
						Mat_<Vec3b> imgCellCamLeft = extractCell(i, j, imgCamLeft);
						Mat_<Vec3b> imgCellCamRight = extractCell(i, j, imgCamRight);
						cells[leftRow][leftCol] = imgCellCamLeft;
						cells[rightRow][rightCol] = imgCellCamRight;
					}
				}

				// classify cells and "build" board
				KNearestNeighborsClassifier classifier(trainImages, 5);
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						std::string classPredicted = classifier.classify(cells[i][j]);

						if (classPredicted == "FR") { sendBuffer[i * 8 + j] = '*'; }
						else if (classPredicted == "WP") { sendBuffer[i * 8 + j] = 'P'; }
						else if (classPredicted == "WB") { sendBuffer[i * 8 + j] = 'B'; }
						else if (classPredicted == "WN") { sendBuffer[i * 8 + j] = 'N'; }
						else if (classPredicted == "WR") { sendBuffer[i * 8 + j] = 'R'; }
						else if (classPredicted == "WQ") { sendBuffer[i * 8 + j] = 'Q'; }
						else if (classPredicted == "WK") { sendBuffer[i * 8 + j] = 'K'; }
						else if (classPredicted == "BP") { sendBuffer[i * 8 + j] = 'p'; }
						else if (classPredicted == "BB") { sendBuffer[i * 8 + j] = 'b'; }
						else if (classPredicted == "BN") { sendBuffer[i * 8 + j] = 'n'; }
						else if (classPredicted == "BR") { sendBuffer[i * 8 + j] = 'r'; }
						else if (classPredicted == "BQ") { sendBuffer[i * 8 + j] = 'q'; }
						else if (classPredicted == "BK") { sendBuffer[i * 8 + j] = 'k'; }
						else
						{
							std::cout << "Unknown class " << classPredicted << std::endl;
							return 1;
						}
					}
				}
			}

			// notify UA about success
			int sendByteCount = send(clientSocket, sendBuffer, 200, 0);
			if (sendByteCount > 0)
			{
				std::cout << "send() success." << std::endl;
			}
			else
			{
				std::cout << "send() error: " << WSAGetLastError();
				WSACleanup();
			}
			continue;
		}
		if (cmd == 't' || cmd == 'T')
		{
			char prefix[20];
			if (cmd == 't')
			{
				strcpy(prefix, "persistence\\train");
			}
			else if (cmd == 'T')
			{
				strcpy(prefix, "persistence\\test");
			}
			else
			{
				std::cout << "Unknown command " << recvBuffer << std::endl;
				continue;
			}

			// left part of board detected by cam1, right part by cam2
			// however, each camera sees what's closer to it as "lower cells" (different perspective)
			Mat_<Vec3b> imgCamLeft = imread(PATH_DUMMY_IMG_CAM_ONE, IMREAD_COLOR);
			Mat_<Vec3b> imgCamRight = imread(PATH_DUMMY_IMG_CAM_TWO, IMREAD_COLOR);

			for (int i = 4; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					// coordinate conversion for naming
					// i, j (coord system of camera x) -> xRow, xCol (main coord system)
					int leftRow, leftCol, rightRow, rightCol;
					leftToMain(i, j, leftRow, leftCol);
					rightToMain(i, j, rightRow, rightCol);

					// path creation
					char leftPath[256];
					char rightPath[256];
					char leftFolder[3];
					char rightFolder[3];

					// encoding given in main coord system by user; gives folder
					mapCharToFolder(recvBuffer[1 + leftRow * 8 + leftCol], leftFolder);
					mapCharToFolder(recvBuffer[1 + rightRow * 8 + rightCol], rightFolder);

					sprintf(leftPath,
						"%s\\%s\\no%02d_row%02d_col%02d%_left.jpeg",
						prefix, leftFolder, currentFileNo, leftRow, leftCol);
					sprintf(rightPath,
						"%s\\%s\\no%02d_row%02d_col%02d%_right.jpeg",
						prefix, rightFolder, currentFileNo, rightRow, rightCol
					);

					// extract and save cell
					Mat_<Vec3b> imgCellCamLeft = extractCell(i, j, imgCamLeft);
					Mat_<Vec3b> imgCellCamRight = extractCell(i, j, imgCamRight);

					if (cv::imwrite(leftPath, imgCellCamLeft) == false)
					{
						std::cout << "Error saving to " << leftPath << std::endl;
						break;
					}
					if (cv::imwrite(rightPath, imgCellCamRight) == false)
					{
						std::cout << "Error saving to " << rightPath << std::endl;
						break;
					}
					std::cout << "Images saved to " << leftPath << " and " << rightPath << std::endl;
				}
			}
			currentFileNo++;

			continue;
		}
		if (cmd == 'r' || cmd == 'R')  // reset knn
		{
			char prefix[20];
			if (cmd == 'r')
			{
				strcpy(prefix, "persistence\\train");
			}
			else if (cmd == 'R')
			{
				strcpy(prefix, "persistence\\test");
			}
			else
			{
				std::cout << "Unknown command " << recvBuffer << std::endl;
				continue;
			}

			char folders[13][3] = { "FR", "WP", "WB", "WN", "WR", "WQ", "WK", "BP", "BB", "BN", "BR", "BQ", "BK" };
			char path[256];
			for (int i = 0; i < 13; i++)
			{
				sprintf(path, "%s\\%s", prefix, folders[i]);

				// delete folder
				std::error_code errorCode;
				if (fs::remove_all(path, errorCode) == -1)
				{
					std::cout << "remove_all: " << errorCode.message() << std::endl;
				}

				// re-create folder
				if (fs::create_directory(path, errorCode) == false)
				{
					std::cout << "create_directory: " << errorCode.message() << std::endl;
				}
			}
		}
		if (cmd == 'x')
		{
			// KNN is a lazy learner, does not have a training phase
			continue;
		}
		if (cmd == 'X')
		{
			const int C = 13;  // number of classes
			const std::array<std::string, C> classes = {
				"black_bishop",
				"black_king",
				"black_knight",
				"black_pawn",
				"black_queen",
				"black_rook",
				"free",
				"white_bishop",
				"white_king",
				"white_knight",
				"white_pawn",
				"white_queen",
				"white_rook"
			};

			std::map<std::string, std::vector<Mat_<Vec3b>>> trainImages;
			std::map<std::string, std::vector<Mat_<Vec3b>>> testImages;

			// insert empty vectors for each class
			for (int i = 0; i < C; i++)
			{
				trainImages.insert(
					std::pair<std::string, std::vector<Mat_<Vec3b>>>(
						classes[i],
						std::vector<Mat_<Vec3b>>()
						));
				testImages.insert(
					std::pair<std::string, std::vector<Mat_<Vec3b>>>(
						classes[i],
						std::vector<Mat_<Vec3b>>()
						));
			}

			char folders[13][3] = { "FR", "WP", "WB", "WN", "WR", "WQ", "WK", "BP", "BB", "BN", "BR", "BQ", "BK" };
			char path[256];
			for (int i = 0; i < 13; i++)
			{
				sprintf(path, "persistence\\train\\%s", folders[i]);
				for (const auto& entry : fs::directory_iterator(path))
				{
					std::string pathString{ entry.path().string() };
					std::cout << pathString << std::endl;

					Mat_<Vec3b> img = imread(pathString, IMREAD_COLOR);
					if (img.empty())
					{
						std::cout << "Could not open file at path: " << pathString << std::endl;
						break;
					}

					// store image
					trainImages[classes[i]].push_back(img);
				}

				sprintf(path, "persistence\\test\\%s", folders[i]);
				for (const auto& entry : fs::directory_iterator(path))
				{
					std::string pathString{ entry.path().string() };
					std::cout << pathString << std::endl;

					Mat_<Vec3b> img = imread(pathString, IMREAD_COLOR);
					if (img.empty())
					{
						std::cout << "Could not open file at path: " << pathString << std::endl;
						break;
					}

					// store image
					testImages[classes[i]].push_back(img);
				}
			}

			// instantiate classifier (includes training)
			KNearestNeighborsClassifier classifier(trainImages, 5);

			// testing phase
			Mat_<double> confusionMatrix(C, C, 0.0);  // row: predicted class  col: actual class
			for (auto const& pair : testImages)
			{
				std::string actualClass = pair.first;           // class
				std::vector<Mat_<Vec3b>> images = pair.second;  // vector of images
				for (Mat_<Vec3b> image : images)
				{
					std::string predictedClass = classifier.classify(image);

					int predictedClassIndex, actualClassIndex;
					for (int i = 0; i < C; i++)
					{
						if (actualClass == classes[i])
						{
							actualClassIndex = i;
						}
						if (predictedClass == classes[i])
						{
							predictedClassIndex = i;
						}
					}

					confusionMatrix(actualClassIndex, predictedClassIndex)++;
				}
			}

			double correct = 0;
			double wrong = 0;
			for (int i = 0; i < C; i++)
			{
				for (int j = 0; j < C; j++)
				{
					if (i == j)
					{
						correct += confusionMatrix(i, j);
					}
					else
					{
						wrong += confusionMatrix(i, j);
					}
				}
			}
			double accuracy = correct / (correct + wrong);

			std::cout << "Train images:" << std::endl;
			int total = 0;
			for (auto const& pair : trainImages)
			{
				total += pair.second.size();
				std::cout << pair.first << ": " << pair.second.size() << std::endl;
			}
			std::cout << "Total: " << total << std::endl << std::endl;

			total = 0;
			std::cout << "Test images:" << std::endl;
			for (auto const& pair : testImages)
			{
				total += pair.second.size();
				std::cout << pair.first << ": " << pair.second.size() << std::endl;
			}
			std::cout << "Total: " << total << std::endl << std::endl;

			std::cout << "Confusion matrix:" << std::endl;
			std::cout << "Classes: ";
			for (int i = 0; i < classes.size(); i++)
			{
				std::cout << " (" << i << ":" << classes[i] << ")";
			}
			std::cout << std::endl << confusionMatrix << std::endl << std::endl;

			std::cout << "Accuracy: " << accuracy << std::endl;
		}
	}

	// close socket
	std::cout << "Calling closesocket" << std::endl;
	closesocket(clientSocket);

	return 0;
}