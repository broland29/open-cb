#pragma once

#include "Board.h"

enum class KingSituation
{
    FREE,
    CHECK,
    CHECKMATE,
    STALEMATE
};

/* Functions in Attack.cpp check if piece can go from one cell to another
 * considering their moving logic and current board metadata. It does not
 * take into consideration any other information. Logic about other
 * additional factors will be considered on a higher level (in Move.cpp).
 */


 /* Checks if every cell from (currRow, currCol) to (destRow, destCol),
    excluding start and end cell. Movement rule (nextRow, nextCol) =
    (currRow + dRow, currCol + dCol). Runs on the premise that we will
    eventually either reach the destination or get out of bounds.

    Return values:
     0 - path is free
     1 - path is not free (could not reach destination)
     2 - we got out of bounds (could not reach destination)
  */
int _checkPath(
    char board[8][8],
    int currRow,
    int currCol,
    int destRow,
    int destCol,
    int dRow,
    int dCol
);

/* CanXYZAttckCell - checks if piece of type XYZ can move from
*  board[currRow][currCol] to board[destRow][destCol]. It only evaluates
*  based on the board setup and on metadata.
*/

// Checks path considering pawn movement
bool _canPawnAttackCell(
    char board[8][8],
    int currRow,
    int currCol,
    int destRow,
    int destCol,
    int enPassantRow
);


bool _canBishopAttackCell(
    char board[8][8],
    int currRow,
    int currCol,
    int destRow,
    int destCol
);


bool _canKnightAttackCell(
    char board[8][8],
    int currRow,
    int currCol,
    int destRow,
    int destCol
);


// Does not handle castling!
bool _canRookAttackCell(
    char board[8][8],
    int currRow,
    int currCol,
    int destRow,
    int destCol
);


bool _canQueenAttackCell(
    char board[8][8],
    int currRow,
    int currCol,
    int destRow,
    int destCol
);


bool _canKingAttackCell(
    char board[8][8],
    int currRow,
    int currCol,
    int destRow,
    int destCol
);


bool canPieceAttackCell(
    char board[8][8],
    int currRow,
    int currCol,
    int destRow,
    int destCol,
    int enPassantCol
);



bool isCellInCheck(
    char board[8][8],
    int row,
    int col,
    Metadata metadata
);


KingSituation getKingSituation(
    char board[8][8],
    Color kingColor,
    Metadata metadata
);