#include "Validator.h"


Validator::Validator()
{
    copyBoard(prevBoard, initialSetup);
    copyBoard(currBoard, initialSetup);
    SPDLOG_TRACE("After construction, validator state:");
    SPDLOG_TRACE(*this);
}


void Validator::validateBoard(char board[8][8], char message[200])
{
    // every board should contain one white and one black king
    int whiteKingCount = 0, blackKingCount = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == WK)
            {
                whiteKingCount++;
            }
            else if (board[i][j] == BK)
            {
                blackKingCount++;
            }
        }
    }
    if (whiteKingCount != 1 || blackKingCount != 1)
    {
        strcpy(message, "IIncorrect king count!");
        return;
    }

    // https://stackoverflow.com/questions/5700204/does-the-implicit-copy-constructor-copy-array-data-members
    Validator oldValidator = *this;

    // update boards
    copyBoard(prevBoard, currBoard);
    copyBoard(currBoard, board);

    // delegate to Move.cpp
    processMove(prevBoard, currBoard, metadata, message);

    // if illegal, undo change
    if (message[0] != 'L')
    {
        *this = oldValidator;
        SPDLOG_INFO("Move is discarded since illegal.");
        return;
    }

    if (metadata.turn == Color::BLACK)
    {
        metadata.moveCount++;
    }
    metadata.changeTurn();

    // if no new pawn move, reset el passant; otherwise let it stay "updated"
    if (oldValidator.metadata.enPassantCol == this->metadata.enPassantCol)
    {
        metadata.enPassantCol = -1;
    }

    SPDLOG_TRACE("After update, validator state:");
    SPDLOG_TRACE(*this);
}


std::ostream& operator<<(std::ostream& os, const Validator& validator)
{
    os << std::endl;
    os << "  currBoard         prevBoard" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        os << "  ";
        for (int j = 0; j < 8; j++)
        {
            os << validator.currBoard[i][j] << " ";
        }
        os << "  ";
        for (int j = 0; j < 8; j++)
        {
            os << validator.prevBoard[i][j] << " ";
        }
        os << std::endl;
    }

    os << std::endl << "  metadata: " << validator.metadata;
    return os;
}