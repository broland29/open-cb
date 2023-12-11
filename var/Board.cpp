#include <iostream>
#include <fstream>

#include "board.h"


void printBoard(char board[8][8], std::ofstream& ofstream)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            ofstream << board[i][j] << " ";
        }
        ofstream << std::endl;
    }
}


void readBoard(char board[8][8], std::ifstream& ifstream)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            ifstream >> board[i][j];
        }
    }
}


void copyBoard(char boardTo[8][8], char boardFrom[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            boardTo[i][j] = boardFrom[i][j];
        }
    }
}


bool isBoardInitialSetup(char board[8][8])
{
    char initialSetup[8][8] = INITIAL_SETUP;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != initialSetup[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
