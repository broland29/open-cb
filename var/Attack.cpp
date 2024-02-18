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
        else if (destRow == currRow - 2)
        {
            // long simple move
            if (destCol == currCol && currRow == 6 && IS_FREE(board[5][destCol]) && IS_FREE(board[4][destCol]))
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
        else if (destRow == currRow + 2)
        {
            // long simple move
            if (destCol == currCol && currRow == 1 && (IS_FREE(board[2][destCol]) && (IS_FREE(board[3][destCol]))))
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


// checks is board[currRow][currCol] can attack board[destRow][destCol]. checks for empty attacker and attacking
// self/ ally as well
bool canPieceAttackCell(char board[8][8], int currRow, int currCol, int destRow, int destCol, int enPassantCol) {
    assert(_isInside(currRow) && _isInside(currCol) && _isInside(destRow) && _isInside(destCol));

    // free cells cannot attack
    if (IS_FREE(board[currRow][currCol]))
    {
        return false;
    }

    // takes care of two cases: attacking ally piece, attacking self
    if (IS_SAME_COLOR(board[currRow][currCol], board[destRow][destCol]))
    {
        return false;
    }

    switch (board[currRow][currCol])
    {
    case WP:
    case BP:
        return _canPawnAttackCell(board, currRow, currCol, destRow, destCol, enPassantCol);
    case WB:
    case BB:
        return _canBishopAttackCell(board, currRow, currCol, destRow, destCol);
    case WR:
    case BR:
        return _canRookAttackCell(board, currRow, currCol, destRow, destCol);
    case WN:
    case BN:
        return _canKnightAttackCell(board, currRow, currCol, destRow, destCol);
    case WQ:
    case BQ:
        return _canQueenAttackCell(board, currRow, currCol, destRow, destCol);
    case WK:
    case BK:
        return _canKingAttackCell(board, currRow, currCol, destRow, destCol);
    default:
        std::cout << "Reached default in canPieceAttackCell!" << std::endl;
        assert(false);
        return false;
    }
}

// Checks if anything can attack cell (row, col)
bool isCellInCheck(char board[8][8], int row, int col, int enPassantCol)
{
    assert(_isInside(row) && _isInside(col));

    // call canPieceAttackCell on each piece
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (canPieceAttackCell(board, i, j, row, col, enPassantCol)) {
                return true;
            }
        }
    }
    return false;
}

struct Attacker
{
    int row;
    int col;
    char enc;
};

std::vector<Attacker> _getAttackers(char board[8][8], int kingRow, int kingCol, int enPassantCol)
{
    std::vector<Attacker> attackers;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (canPieceAttackCell(board, i, j, kingRow, kingCol, enPassantCol))
            {
                attackers.push_back(Attacker{i, j, board[i][j]});
            }
        }
    }

    return attackers;
}

bool _getCanKingMove(char board[8][8], int kingRow, int kingCol, int enPassantCol)
{
    int di[8] = { -1, -1,  0,  1,  1,  1,  0, -1 };
    int dj[8] = {  0,  1,  1,  1,  0, -1, -1, -1 };

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
        if (!IS_ATTACKABLE(board[kingRow][kingCol], board[destRow][destCol]))
        {
            continue;
        }

        // build up setup for hypothetical move
        char auxBoard[8][8];
        copyBoard(auxBoard, board);
        auxBoard[destRow][destCol] = board[kingRow][kingCol];
        auxBoard[kingRow][kingCol] = FR;

        // skip cell which would put king in check
        if (isCellInCheck(auxBoard, destRow, destCol, enPassantCol))
        {
            continue;
        }

        // cell board[destRow][destCol] is safe
        return true;
    }

    return false;
}

// can other piece of the same color as the king, except the king, move
bool _getCanOtherMove(char board[8][8], int kingRow, int kingCol, int enPassantCol)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // skip king
            if (i == kingRow && j == kingCol)
            {
                continue;
            }

            // skip enemy piece
            if (!IS_SAME_COLOR(board[kingRow][kingCol], board[i][j]))
            {
                continue;
            }

            for (int u = 0; u < 8; u++)
            {
                for (int v = 0; v < 8; v++)
                {
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

bool _getCanAttackerBeBlocked(char board[8][8], int kingRow, int kingCol, int enPassantCol, std::vector<Attacker> attackers)
{
    // no attackers
    if (attackers.size() == 0)
    {
        return true;
    }

    // if just one attacker, maybe it can be captured
    if (attackers.size() == 1)
    {
        int attackerRow = attackers[0].row;
        int attackerCol = attackers[0].col;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (canPieceAttackCell(board, i, j, attackerRow, attackerCol, enPassantCol))
                {
                    return true;
                }
            }
        }
    }

    // if more attackers, the only option is to move a piece such that it blocks each attack (could work for one attacker as well)
    int attackingPath[8][8] = { 0 };

    for (const Attacker &attacker : attackers)
    {
        // reconstruct attacker path
        int rowDifference, colDifference, rowStep, colStep, attackerRow, attackerCol;
        switch (attacker.enc)
        {
            // these have "attack range of 1" (or knight), cannot be blocked
            case WP:
            case BP:
            case WN:
            case BN:
            case WK:
            case BK:
                return false;
            // these have linear attacks which might be blockable ("longer than 1")
            case WB:
            case BB:
            case WR:
            case BR:
            case WQ:
            case BQ:
                attackerRow = attacker.row;
                attackerCol = attacker.col;
                rowDifference = kingRow - attackerRow;
                colDifference = kingCol - attackerCol;

                assert(
                    abs(rowDifference) == abs(colDifference) ||  // diagonal
                    rowDifference != 0 && colDifference == 0 ||  // horizontal
                    colDifference != 0 && rowDifference == 0     // vertical
                );

                // check if attacker near king -> not blockable
                if (abs(rowDifference) == 1 || abs(colDifference) == 1)
                {
                    return false;
                }

                // calculate steps
                if (rowDifference < 0)
                {
                    rowStep = -1;
                }
                else if (rowDifference > 0)
                {
                    rowStep = +1;
                }
                else
                {
                    rowStep = 0;
                }
                if (colDifference < 0)
                {
                    colStep = -1;
                }
                else if (colDifference > 0)
                {
                    colStep = +1;
                }
                else
                {
                    colStep = 0;
                }

                // move attacker step by step and add to path
                // extreme case: king and attacker at 8 step difference
                // in this case, 6 intermediary cells must be added to the path
                //   => there shall be no more than 6 steps to reach king
                int i;

                // step out of original location
                attackerRow += rowStep;
                attackerCol += colStep;

                for (i = 0; i < 6; i++)
                {
                    assert(attackerRow >= 0 && attackerRow <= 7);
                    assert(attackerCol >= 0 && attackerCol <= 7);

                    attackingPath[attackerRow][attackerCol]++;
                    attackerRow += rowStep;
                    attackerCol += colStep;

                    // king cell shall not be added, break before
                    if (attackerRow == kingRow && attackerCol == kingCol)
                    {
                        break;
                    }
                }
                assert(i != 6);  // we shall always brake out before reaching end condition of for loop
                break;
        }
    }

    int crossRow = -1, crossCol;  // attacking paths may cross each other, if there is one such crossing, it may be blocked
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (attackingPath[i][j] > 1)
            {
                if (crossRow != -1)
                {
                    return false;  // there already is a cross point, two such points mean its impossible to block
                }
                crossRow = i;
                crossCol = j;
            }
        }
    }

    // could do fancier logic, but for now, just try blocking each attacking path
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // not attacking path, not of interest
            if (attackingPath[i][j] == 0)
            {
                continue;
            }

            for (int u = 0; u < 8; u++)
            {
                for (int v = 0; v < 8; v++)
                {
                    if (!canPieceAttackCell(board, u, v, i, j, -1))
                    {
                        continue;
                    }

                    // build up setup for hypothetical move
                    char auxBoard[8][8];
                    copyBoard(auxBoard, board);
                    auxBoard[u][v] = board[i][j];
                    auxBoard[i][j] = FR;

                    if (!isCellInCheck(auxBoard, kingRow, kingCol, -1))
                    {
                        return true;  // found a solution!
                    }
                }
            }
        }
    }

    return false;
}


KingSituation getKingSituation(char board[8][8], Color kingColor, int enPassantCol)
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

    std::vector<Attacker> attackers = _getAttackers(board, kingRow, kingCol, enPassantCol);
    bool canKingMove = _getCanKingMove(board, kingRow, kingCol, enPassantCol);
    bool canOtherMove = _getCanOtherMove(board, kingRow, kingCol, enPassantCol);
    bool canAttackersBeBlocked = _getCanAttackerBeBlocked(board, kingRow, kingCol, enPassantCol, attackers);

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
