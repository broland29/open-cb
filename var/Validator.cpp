#include "Validator.h"

Validator::Validator()
{
    copyBoard(prevBoard, initialSetup);
    copyBoard(currBoard, initialSetup);
}

void Validator::validateBoard(char board[8][8], char message[200])
{
    // every board should contain both kings once
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

    // todo - how to deal with illegal move? undo operation? need to restore state

    copyBoard(prevBoard, currBoard);
    copyBoard(currBoard, board);

    int oldEnPassantCol = metadata.enPassantCol;
    processMove(prevBoard, currBoard, metadata, message);
    int newEnPassantCol = metadata.enPassantCol;

    if (message[0] != 'L')  // legal move
    {
        if (metadata.turn == Color::BLACK)
        {
            metadata.moveCount++;
        }

        if (oldEnPassantCol == newEnPassantCol)
        {
            metadata.enPassantCol = -1;  // new pawn did not move, reset el passant; otherwise let it stay updated
        }

        metadata.changeTurn();
    }
}