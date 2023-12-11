#include <iostream>
#include <assert.h>

#include "Attack.h"


// since board is 8x8, can be hardcoded both for row and column
bool _isInside(int coord)
{
    return coord >= 0 && coord < 8;
}


int _checkPath(char board[8][8], int currRow, int currCol, int destRow, int destCol, int dRow, int dCol)
{
    assert(_isInside(currRow) && _isInside(currCol) && _isInside(destRow) && _isInside(destCol));

    while (true)
    {
        currRow += dRow;
        currCol += dCol;

        // don't care if destination is taken!
        if (currRow == destRow && currCol == destCol)
        {
            return 0;
        }

        // check before accessing element!
        if (!_isInside(currRow) || !_isInside(currCol))
        {
            return 2;
        }

        if (IS_NOT_FREE(board[currRow][currCol]))
        {
            return 1;
        }
    }
}



bool _canPawnAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol, int enPassantCol)
{
    assert(IS_WHITE_PAWN(board[currRow][currCol]) || IS_BLACK_PAWN(board[currRow][currCol]));

    if (IS_WHITE_PAWN(board[currRow][currCol]))
    {
        if (destRow == currRow - 1)
        {
            // simple white move
            if (destCol == currCol && IS_FREE(board[destRow][destCol]))
            {
                return true;
            }
            // diagonal move
            if (destCol == currCol - 1 || destCol == currCol + 1)
            {
                // en passant
                if (currRow == 3 && enPassantCol == destCol)
                {
                    return true;
                }

                // simple capture
                if (IS_BLACK(board[destRow][destCol]))
                {
                    return true;
                }
            }
        }

        return false;
    }
    else
    {
        if (destRow == currRow + 1)
        {
            // simple black move
            if (destCol == currCol && IS_FREE(board[destRow][destCol]))
            {
                return true;
            }
            // diagonal move
            if (destCol == currCol - 1 || destCol == currCol + 1)
            {
                // en passant
                if (currRow == 4 && enPassantCol == destCol)
                {
                    return true;
                }

                // simple capture
                if (IS_WHITE(board[destRow][destCol]))
                {
                    return true;
                }
            }
        }

        return false;
    }
}

// Checks path considering bishop movement
bool _canBishopAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    if (abs(destRow - currRow) != abs(destCol - currCol))
    {
        return false;  // not diagonal, cannot reach, not worth trying
    }

    if (destRow > currRow)
    {
        if (destCol > currCol)  // go down-right
        {
            return _checkPath(board, currRow, currCol, destRow, destCol, +1, +1) == 0;
        }
        else                    //go down-left
        {
            return _checkPath(board, currRow, currCol, destRow, destCol, +1, -1) == 0;
        }
    }
    else
    {
        if (destCol > currCol)  // go up-right
        {
            return _checkPath(board, currRow, currCol, destRow, destCol, -1, +1) == 0;
        }
        else                    // go up-left
        {
            return _checkPath(board, currRow, currCol, destRow, destCol, -1, -1) == 0;
        }
    }
}


// Checks path considering knight movement
bool _canKnightAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    // cases clockwise
    if (destRow == currRow - 2 && destCol == currCol + 1 ||
        destRow == currRow - 1 && destCol == currCol + 2 ||
        destRow == currRow + 1 && destCol == currCol + 2 ||
        destRow == currRow + 2 && destCol == currCol + 1 ||
        destRow == currRow + 2 && destCol == currCol - 1 ||
        destRow == currRow + 1 && destCol == currCol - 2 ||
        destRow == currRow - 1 && destCol == currCol - 2 ||
        destRow == currRow - 2 && destCol == currCol - 1)
    {
        return true;
    }
    return false;
}


// Checks path considering rook movement
bool _canRookAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    if (destRow == currRow)
    {
        if (destCol > currCol)  // go right
        {
            return _checkPath(board, currRow, currCol, destRow, destCol, 0, +1) == 0;
        }
        else                    // go left
        {
            return _checkPath(board, currRow, currCol, destRow, destCol, 0, -1) == 0;
        }
    }
    else if (destCol == currCol)
    {
        if (destRow > currRow)  // go down
        {
            return _checkPath(board, currRow, currCol, destRow, destCol, +1, 0) == 0;
        }
        else                    // go up
        {
            return _checkPath(board, currRow, currCol, destRow, destCol, -1, 0) == 0;
        }
    }
    else
    {
        return false;  // not straight, cannot reach, not worth trying
    }
}


// Checks path considering queen movement
bool _canQueenAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    return _canBishopAttackCell(board, currRow, currCol, destRow, destCol) ||
        _canRookAttackCell(board, currRow, currCol, destRow, destCol);
}


// Checks path considering queen movement
bool _canKingAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    // cases clockwise
    if (destRow == currRow - 1 && destCol == currCol ||
        destRow == currRow - 1 && destCol == currCol + 1 ||
        destRow == currRow && destCol == currCol + 1 ||
        destRow == currRow + 1 && destCol == currCol + 1 ||
        destRow == currRow + 1 && destCol == currCol ||
        destRow == currRow + 1 && destCol == currCol - 1 ||
        destRow == currRow && destCol == currCol - 1 ||
        destRow == currRow - 1 && destCol == currCol - 1)
    {
        return true;
    }
    return false;
}

// this only does step "can move be done"
bool canPieceAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol, Metadata metadata) {
    // validate input
    if (!_isInside(currRow) || !_isInside(currCol) || !_isInside(destRow) || !_isInside(destCol))
    {
        return false;
    }

    char piece = board[currRow][currCol];

    switch (piece)
    {
    case 'P':
    case 'p':
        return _canPawnAttackCell(board, currRow, currCol, destRow, destCol, metadata.enPassantCol);
    case 'B':
    case 'b':
        return _canBishopAttackCell(board, currRow, currCol, destRow, destCol);
    case 'R':
    case 'r':
        return _canRookAttackCell(board, currRow, currCol, destRow, destCol);
    case 'n':
    case 'N':
        return _canKnightAttackCell(board, currRow, currCol, destRow, destCol);
    case 'Q':
    case 'q':
        return _canQueenAttackCell(board, currRow, currCol, destRow, destCol);
    case 'K':
    case 'k':
        return _canKingAttackCell(board, currRow, currCol, destRow, destCol);
    default:
        std::cout << "Reached default in canPieceAttackCell!" << std::endl;
        return false;
    }
}

// Checks if anything can attack cell (row, col)
bool isCellInCheck(char board[8][8], int row, int col, Metadata metadata)
{
    // call canPieceAttackCell on each piece
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i == row && j == col)
            {
                continue;  // should not check if piece can attack itself!
            }
            if (IS_NOT_FREE(board[i][j]))
            {
                //if (canPieceAttackCell(board, i, j, row, col, metadata, false)) {
                //    return true;  TODO
                //}
            }
        }
    }
    return false;
}


// Checks if king is in check
bool isKingInCheck(char board[8][8], Color kingColor, Metadata metadata)
{
    int kingRow, kingCol;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (kingColor == Color::WHITE && IS_WHITE_KING(board[i][j]))
            {
                kingRow = i;
                kingCol = j;
                goto _found;
            }
            if (kingColor == Color::BLACK && IS_BLACK_KING(board[i][j]))
            {
                kingRow = i;
                kingCol = j;
                goto _found;
            }
        }
    }
_found:
    return isCellInCheck(board, kingRow, kingCol, metadata);
}

bool isKingInCheckmate(char board[8][8], Color kingColor, Metadata metadata)
{
    int kingRow, kingCol;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (kingColor == Color::WHITE && IS_WHITE_KING(board[i][j]))
            {
                kingRow = i;
                kingCol = j;
                goto _found;
            }
            if (kingColor == Color::BLACK && IS_BLACK_KING(board[i][j]))
            {
                kingRow = i;
                kingCol = j;
                goto _found;
            }
        }
    }
_found:

    // offset for current cell and all neighboring cells
    int di[] = { 0, -1, -1,  0,  1,  1,  1,  0, -1 };
    int dj[] = { 0,  0,  1,  1,  1,  0, -1, -1, -1 };
    for (int i = 0; i < 9; i++)
    {
        int ci = kingRow + di[i];
        int cj = kingCol + dj[i];

        // skip invalid cells
        if (ci < 0 || ci > 8 || cj < 0 || cj > 0)
        {
            continue;
        }

        // if king is not in check or there is one possibility to escape
        if (!isCellInCheck(board, ci, cj, metadata))
        {
            return false;
        }
    }
    return true;
}