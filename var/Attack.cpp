#include <iostream>
#include <assert.h>

#include "Attack.h"
#include <vector>


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
    assert(_isInside(currRow) && _isInside(currCol) && _isInside(destRow) && _isInside(destCol));
    assert(enPassantCol >= -1 && enPassantCol <= 7);

    if (IS_WHITE_PAWN(board[currRow][currCol]))
    {
        if (destRow == currRow - 1)
        {
            // simple move
            if (destCol == currCol && IS_FREE(board[destRow][destCol]))
            {
                return true;
            }

            // en passant left
            if (destCol == currCol - 1 && IS_BLACK_PAWN(board[currRow][currCol - 1]) && currRow == 3 && enPassantCol == destCol && IS_FREE(board[destRow][destCol]))
            {
                return true;
            }

            // en passant right
            if (destCol == currCol + 1 && IS_BLACK_PAWN(board[currRow][currCol + 1]) && currRow == 3 && enPassantCol == destCol && IS_FREE(board[destRow][destCol]))
            {
                return true;
            }

            // simple capture
            if ((destCol == currCol - 1 || destCol == currCol + 1) && IS_BLACK(board[destRow][destCol]))
            {
                return true;
            }
        }
    }
    else if (IS_BLACK_PAWN(board[currRow][currCol]))
    {
        if (destRow == currRow + 1)
        {
            // simple move
            if (destCol == currCol && IS_FREE(board[destRow][destCol]))
            {
                return true;
            }

            // en passant left
            if (destCol == currCol - 1 && IS_WHITE_PAWN(board[currRow][currCol - 1]) && currRow == 4 && enPassantCol == destCol)
            {
                return true;
            }

            // en passant right
            if (destCol == currCol + 1 && IS_WHITE_PAWN(board[currRow][currCol + 1]) && currRow == 4 && enPassantCol == destCol)
            {
                return true;
            }

            // simple capture
            if ((destCol == currCol - 1 || destCol == currCol + 1) && IS_WHITE(board[destRow][destCol]))
            {
                return true;
            }
        }
    }
    return false;
}


bool _canBishopAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    assert(
        IS_WHITE_BISHOP(board[currRow][currCol]) || IS_BLACK_BISHOP(board[currRow][currCol]) ||
        IS_WHITE_QUEEN(board[currRow][currCol])  || IS_BLACK_QUEEN(board[currRow][currCol]));  // also used for queen movement
    assert(_isInside(currRow) && _isInside(currCol) && _isInside(destRow) && _isInside(destCol));

    if (abs(destRow - currRow) != abs(destCol - currCol))
    {
        return false;  // not diagonal, cannot reach, not worth trying
    }

    bool isPathFree = false;
    if (destRow > currRow)
    {
        if (destCol > currCol)  // go down-right
        {
            isPathFree = (_checkPath(board, currRow, currCol, destRow, destCol, +1, +1) == 0);
        }
        else                    //go down-left
        {
            isPathFree = (_checkPath(board, currRow, currCol, destRow, destCol, +1, -1) == 0);
        }
    }
    else
    {
        if (destCol > currCol)  // go up-right
        {
            isPathFree = (_checkPath(board, currRow, currCol, destRow, destCol, -1, +1) == 0);
        }
        else                    // go up-left
        {
            isPathFree = (_checkPath(board, currRow, currCol, destRow, destCol, -1, -1) == 0);
        }
    }
    return isPathFree && (IS_ATTACKABLE(board[currRow][currCol], board[destRow][destCol]));
}


bool _canKnightAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    assert(IS_WHITE_KNIGHT(board[currRow][currCol]) || IS_BLACK_KNIGHT(board[currRow][currCol]));
    assert(_isInside(currRow) && _isInside(currCol) && _isInside(destRow) && _isInside(destCol));

    // offsets clockwise
    int di[8] = { -2, -1,  1,  2,  2,  1, -1, -2 };
    int dj[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };

    bool respectsRule = false;
    for (int i = 0; i < 8; i++)
    {
        if (destRow == currRow + di[i] && destCol == currCol + dj[i])
        {
            respectsRule = true;
            break;
        }
    }

    return respectsRule && (IS_ATTACKABLE(board[currRow][currCol], board[destRow][destCol]));
}


// Checks path considering rook movement
bool _canRookAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    assert(
        IS_WHITE_ROOK(board[currRow][currCol]) || IS_BLACK_ROOK(board[currRow][currCol]) ||
        IS_WHITE_QUEEN(board[currRow][currCol]) || IS_BLACK_QUEEN(board[currRow][currCol]));  // also used for queen movement
    assert(_isInside(currRow) && _isInside(currCol) && _isInside(destRow) && _isInside(destCol));

    bool isPathFree = false;
    if (destRow == currRow)
    {
        if (destCol > currCol)  // go right
        {
            isPathFree = (_checkPath(board, currRow, currCol, destRow, destCol, 0, +1) == 0);
        }
        else                    // go left
        {
            isPathFree = (_checkPath(board, currRow, currCol, destRow, destCol, 0, -1) == 0);
        }
    }
    else if (destCol == currCol)
    {
        if (destRow > currRow)  // go down
        {
            isPathFree = (_checkPath(board, currRow, currCol, destRow, destCol, +1, 0) == 0);
        }
        else                    // go up
        {
            isPathFree = (_checkPath(board, currRow, currCol, destRow, destCol, -1, 0) == 0);
        }
    }
    return isPathFree && (IS_ATTACKABLE(board[currRow][currCol], board[destRow][destCol]));
}


// Checks path considering queen movement
bool _canQueenAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    return
        (_canBishopAttackCell(board, currRow, currCol, destRow, destCol) ||
         _canRookAttackCell(board, currRow, currCol, destRow, destCol)) &&
        (IS_ATTACKABLE(board[currRow][currCol], board[destRow][destCol]));
}

// Checks path considering queen movement
bool _canKingAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol)
{
    assert(IS_WHITE_KING(board[currRow][currCol]) || IS_BLACK_KING(board[currRow][currCol]));
    assert(_isInside(currRow) && _isInside(currCol) && _isInside(destRow) && _isInside(destCol));

    // offsets clockwise
    int di[8] = { -1, -1,  0,  1,  1,  1,  0, -1 };
    int dj[8] = {  0,  1,  1,  1,  0, -1, -1, -1 };

    bool respectsRule = false;
    for (int i = 0; i < 8; i++)
    {
        if (destRow == currRow + di[i] && destCol == currCol + dj[i])
        {
            respectsRule = true;
            break;
        }
    }

    return respectsRule && (IS_ATTACKABLE(board[currRow][currCol], board[destRow][destCol]));
}


bool canPieceAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol, int enPassantCol) {
    assert(_isInside(currRow) && _isInside(currCol) && _isInside(destRow) && _isInside(destCol));

    switch (board[currRow][currCol])
    {
    case 'P':
    case 'p':
        return _canPawnAttackCell(board, currRow, currCol, destRow, destCol, enPassantCol);
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
        assert(false);
        return false;
    }
}

// Checks if anything can attack cell (row, col)
bool isCellInCheck(char board[8][8], int row, int col, Metadata metadata)
{
    assert(_isInside(row) && _isInside(col));

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
                if (canPieceAttackCell(board, i, j, row, col, metadata.enPassantCol)) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<std::pair<int, int>> _getAttackers(char board[8][8], int kingRow, int kingCol, int enPassantCol)
{
    std::vector<std::pair<int, int>> attackers;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // skip king
            if (i == kingRow && j == kingCol)
            {
                continue;
            }

            if (canPieceAttackCell(board, i, j, kingRow, kingCol, enPassantCol))
            {
                attackers.push_back(std::pair<int, int>(i, j));
            }
        }
    }

    return attackers;
}

bool _getCanKingMove(char board[8][8], int kingRow, int kingCol, Metadata metadata)
{
    int di[8] = { -1, -1,  0,  1,  1,  1,  0, -1 };
    int dj[8] = { 0,  1,  1,  1,  0, -1, -1, -1 };

    for (int i = 0; i < 8; i++)
    {
        int destRow = kingRow + di[i];
        int destCol = kingCol + dj[i];

        // skip OOB cells
        if (!_isInside(destRow) || !_isInside(destCol))
        {
            continue;
        }

        // skip occupied cells
        if (!(IS_ATTACKABLE(board[kingRow][kingCol], board[destRow][destCol])))
        {
            continue;
        }

        // build up setup for hypothetical move
        char auxBoard[8][8];
        copyBoard(auxBoard, board);
        auxBoard[destRow][destCol] = board[kingRow][kingCol];
        auxBoard[kingRow][kingCol] = FR;

        // skip cell which would put king in check
        if (isCellInCheck(auxBoard, destRow, destCol, metadata))
        {
            continue;
        }

        // cell board[destRow][destCol] is safe
        return true;
    }

    return false;
}

bool _getCanOtherMove(char board[8][8], int kingRow, int kingCol, Color kingColor, int enPassantCol)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // TODO - get color function?
            // skip enemy piece
            if (kingColor == Color::BLACK && (IS_WHITE(board[i][j])) || kingColor == Color::WHITE && (IS_BLACK(board[i][j])))
            {
                continue;
            }

            for (int u = 0; u < 8; u++)
            {
                for (int v = 0; v < 8; v++)
                {
                    // skip self
                    if (u == i && v == j)
                    {
                        continue;
                    }
                    if (canPieceAttackCell(board, u, v, i, j, enPassantCol))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool _getCanAttackerBeBlocked(char board[8][8], int kingRow, int kingCol, Color kingColor, int enPassantCol, std::vector<std::pair<int, int>> attackers)
{
    // no attackers
    if (attackers.size() == 0)
    {
        return true;
    }

    // if just one attacker, maybe it can be captured
    if (attackers.size() == 1)
    {
        int attackerRow = attackers[0].first;
        int attackerCol = attackers[0].second;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                // TODO - get color function?
                // skip enemy piece
                if (kingColor == Color::BLACK && (IS_WHITE(board[i][j])) || kingColor == Color::WHITE && (IS_BLACK(board[i][j])))
                {
                    continue;
                }

                // skip self
                if (i == attackerRow && j == attackerCol)
                {
                    continue;
                }

                if (canPieceAttackCell(board, i, j, attackerRow, attackerCol, enPassantCol))
                {
                    return true;
                }
            }
        }
    }

    // if more attackers, the only option is to move a piece such that it blocks each attack (could work for one attacker as well)
    for (std::pair<int, int> attacker : attackers)
    {
        int attackerRow = attackers[0].first;
        int attackerCol = attackers[0].second;

        // no way to block knight
        if (IS_WHITE_KNIGHT(board[attackerRow][attackerCol]) || IS_BLACK_KNIGHT(board[attackerRow][attackerCol]))
        {
            return false;
        }

        return true;  // TODO implement
    }

    return false;
}


KingSituation getKingSituation(char board[8][8], Color kingColor, Metadata metadata)
{
    // get king position
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
    std::cout << "King not found!" << std::endl;
    assert(false);
_found:

    std::vector<std::pair<int, int>> attackers = _getAttackers(board, kingRow, kingCol, metadata.enPassantCol);
    bool canKingMove = _getCanKingMove(board, kingRow, kingCol, metadata);
    bool canOtherMove = _getCanOtherMove(board, kingRow, kingCol, kingColor, metadata.enPassantCol);
    bool canAttackersBeBlocked = _getCanAttackerBeBlocked(board, kingRow, kingCol, kingColor, metadata.enPassantCol, attackers);

    if (attackers.size() == 0)
    {
        if (canKingMove || canOtherMove)
        {
            return KingSituation::FREE;
        }
        return KingSituation::STALEMATE;
    }
    else  // we have attackers, we are in check
    {
        if (canKingMove || canAttackersBeBlocked)
        {
            return KingSituation::CHECK;
        }
        return KingSituation::CHECKMATE;
    }
}
