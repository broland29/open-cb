#include "Validator.h"

Validator::Validator()
{
    copyBoard(prevBoard, initialSetup);
    copyBoard(currBoard, initialSetup);
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
    std::cout << "\n\n1:\n" << *this;
    copyBoard(prevBoard, currBoard);
    std::cout << "\n\n2:\n" << *this;
    copyBoard(currBoard, board);
    std::cout << "\n\n3:\n" << *this <<"\n\n";
    std::cout << "In validateBoard(), before processMove:" << std::endl << *this;

    processMove(prevBoard, currBoard, metadata, message);

    // if illegal, redo
    if (message[0] != 'L')
    {
        *this = oldValidator;
        std::cout << "Invalid move undo" << std::endl;
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

    std::cout << "In validateBoard(), at end:" << std::endl << *this;
}

std::ostream& operator<<(std::ostream& os, const Validator& validator)
{
    os << "currBoard:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            os << validator.currBoard[i][j] << " ";
        }
        os << std::endl;
    }
    os << "prevBoard:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            os << validator.prevBoard[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}