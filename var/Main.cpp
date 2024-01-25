#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sys/stat.h>

#include "Attack.h"
#include "Board.h"
#include "Main.h"
#include "Move.h"

#pragma warning(disable : 4996)



#include "Server.h"

int main()
{
    return server_main();

    /*
    std::ofstream log("log.txt");
    if (!log.is_open())
    {
        std::cout << "Could not open log." << std::endl;
        return 1;
    }

    char folderName[256];
    char resultName[256];
    char fileName[256];
    int folderCount = 0;
    int fileCount;

    while (true)
    {
        sprintf(folderName, "test\\test_main\\test_%03d", folderCount++);
        log << "Folder name: " << folderName << std::endl;
        struct stat buffer;
        if (stat(folderName, &buffer) != 0)
        {
            log << "Could not open folder " << folderName << "." << std::endl;
            break;
        }

        sprintf(resultName, "%s\\result.txt", folderName);
        log << "Result name: " << resultName << std::endl;
        std::ofstream result(resultName);
        if (!result.is_open())
        {
            log << "Could not open result (" << resultName << ")." << std::endl;
            break;
        }

        fileCount = 0;
        Metadata metadata;
        char prevBoard[8][8];
        char currBoard[8][8];

        while (true)
        {
            sprintf(fileName, "%s\\board_%03d.txt", folderName, fileCount++);
            log << "File name: " << fileName << std::endl;
            std::ifstream file(fileName);
            if (!file.is_open())
            {
                log << "Could not open file " << fileName << "." << std::endl;
                result.close();
                break;
            }

            if (fileCount == 0)
            {
                readBoard(currBoard, file);
                printBoard(currBoard, log);
                if (isBoardInitialSetup(currBoard))
                {
                    continue;  // take next board
                }

                result.close();
                break;  // illegal setup, run over
            }
            else
            {
                copyBoard(prevBoard, currBoard);
                readBoard(currBoard, file);
                printBoard(currBoard, log);

                char encoding[10];
                char message[100];
                processMove(prevBoard, currBoard, metadata, message, encoding, log);
                std::cout << "In main: " << encoding << std::endl;
                if (strcmp(message, S_SUCCESS))
                {
                    if (metadata.turn == Color::WHITE)
                    {
                        result << metadata.moveCount << ". " << encoding << " ";
                    }
                    else
                    {
                        result << encoding << std::endl;
                        metadata.moveCount++;
                    }
                    metadata.changeTurn();
                }
                else
                {
                    result << message << std::endl;
                    result.close();
                    break;  // bad move, run over
                }

                // todo - if en passant made possible next round, set it
                // if not, set -1
            }
        }
    }

    log << "Closing log." << std::endl;
    log.close();
    */
}
