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
    processMove(prevBoard, currBoard, metadata, message);
    if (message[0] != 'i')  // legal move
    {
        if (metadata.turn == Color::BLACK)
        {
            metadata.moveCount++;
        }
        metadata.changeTurn();
    }
}