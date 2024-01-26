#include "Validator.h"

Validator::Validator()
{
    copyBoard(prevBoard, initialSetup);
    copyBoard(currBoard, initialSetup);
}

void Validator::validateBoard(char board[8][8], char message[200])
{
    copyBoard(prevBoard, currBoard);
    copyBoard(currBoard, board);

    int oldEnPassantCol = metadata.enPassantCol;
    processMove(prevBoard, currBoard, metadata, message);
    int newEnPassantCol = metadata.enPassantCol;

    if (message[0] != 'i')  // legal move
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