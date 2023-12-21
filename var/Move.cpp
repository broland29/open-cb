#include "Move.h"

bool _isBlackMove(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    // --- turn check --- //
    if (metadata.turn != Color::BLACK)
    {
        return false;
    }

    // --- setup --- //
    Change a, b;
    int aCount = 0, bCount = 0;

    for (int i = 0; i < 2; i++)
    {
        if (changes[i].wasBlack() && changes[i].isFree())
        {
            a = changes[i];
            aCount++;
        }
        else if (changes[i].wasFree() && changes[i].isBlack())
        {
            b = changes[i];
            bCount++;
        }
    }

    if (aCount != 1 || bCount != 1)
    {
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        return false;
    }

    // --- special checks --- //
    // simple move
    if (a.prev == b.curr)
    {
        // check if move affecting castling or en passant
        if (a.wasBlackKing())
        {
            metadata.castle.didBlackKingMove = true;
        }
        else if (a.wasBlackRook() && a.row == 0)
        {
            if (a.col == 0)
            {
                metadata.castle.didBlackQueensideRookMove = true;
            }
            else if (a.col == 7)
            {
                metadata.castle.didBlackKingsideRookMove = true;
            }
        }
        else if (a.wasBlackPawn() && a.row == 1 && b.row == 3)
        {
            /* TODO
            metadata.enPassant.isPossible = true;
            metadata.enPassant.row = b.row;
            metadata.enPassant.col = b.col;
            */
        }

        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            false, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        return true;
    }

    // promotion
    if (a.wasBlackPawn() && (b.isBlackBishop() || b.isBlackKnight() || b.isBlackRook() || b.isBlackQueen()))
    {
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            false, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, true, b.curr, encoding);
        return true;
    }

    return false;
}

bool _isWhiteMove(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    // --- turn check --- //
    if (metadata.turn != Color::WHITE)
    {
        return false;
    }

    // --- setup --- //
    Change a, b;
    int aCount = 0, bCount = 0;

    for (int i = 0; i < 2; i++)
    {
        if (changes[i].wasWhite() && changes[i].isFree())
        {
            a = changes[i];
            aCount++;
        }
        else if (changes[i].wasFree() && changes[i].isWhite())
        {
            b = changes[i];
            bCount++;
        }
    }

    if (aCount != 1 || bCount != 1)
    {
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        return false;
    }

    // --- special checks --- //
    // simple move
    if (a.prev == b.curr)
    {
        // check if move affecting castling or en passant
        if (a.wasWhiteKing())
        {
            metadata.castle.didWhiteKingMove = true;
        }
        else if (a.wasWhiteRook() && a.row == 7)
        {
            if (a.col == 0)
            {
                metadata.castle.didWhiteQueensideRookMove = true;
            }
            else if (a.col == 7)
            {
                metadata.castle.didWhiteKingsideRookMove = true;
            }
        }
        else if (a.wasWhitePawn() && a.row == 6 && b.row == 4)
        {
            /* TODO
            metadata.enPassant.isPossible = true;
            metadata.enPassant.row = b.row;
            metadata.enPassant.col = b.col;
            */
        }

        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            false, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        std::cout << "In isWhiteMove: " << encoding << std::endl;
        return true;
    }

    // promotion
    if (a.wasWhitePawn() && (b.isWhiteBishop() || b.isWhiteKnight() || b.isWhiteRook() || b.isWhiteQueen()))
    {
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            false, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, true, b.curr, encoding);
        return true;
    }

    return false;
}

bool _isBlackCapture(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    // --- turn check --- //
    if (metadata.turn != Color::BLACK)
    {
        return false;
    }

    // --- setup --- //
    Change a, b;
    int aCount = 0, bCount = 0;

    for (int i = 0; i < 2; i++)
    {
        if (changes[i].wasBlack() && changes[i].isFree())
        {
            a = changes[i];
            aCount++;
        }
        else if (changes[i].wasWhite() && changes[i].isBlack())
        {
            b = changes[i];
            bCount++;
        }
    }

    if (aCount != 1 || bCount != 1)
    {
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        return false;
    }

    // --- special checks --- //
    // simple capture
    if (a.prev == b.curr)
    {
        // check if move affecting castling
        if (a.wasBlackKing())
        {
            metadata.castle.didBlackKingMove = true;
        }
        else if (a.wasBlackRook() && a.row == 0)
        {
            if (a.col == 0)
            {
                metadata.castle.didBlackQueensideRookMove = true;
            }
            else if (a.col == 7)
            {
                metadata.castle.didBlackKingsideRookMove = true;
            }
        }

        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        return true;
    }

    // promotion
    if (a.wasBlackPawn() && (b.isBlackBishop() || b.isBlackKnight() || b.isBlackRook() || b.isBlackQueen()))
    {
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, true, b.curr, encoding);
        return true;
    }

    return false;
}

bool _isWhiteCapture(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    // --- turn check --- //
    if (metadata.turn != Color::WHITE)
    {
        return false;
    }

    // --- setup --- //
    Change a, b;
    int aCount = 0, bCount = 0;

    for (int i = 0; i < 2; i++)
    {
        if (changes[i].wasWhite() && changes[i].isFree())
        {
            a = changes[i];
            aCount++;
        }
        else if (changes[i].wasBlack() && changes[i].isWhite())
        {
            b = changes[i];
            bCount++;
        }
    }

    if (aCount != 1 || bCount != 1)
    {
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        return false;
    }

    // --- special checks --- //
    // simple  capture
    if (a.prev == b.curr)
    {
        // check if move affecting castling
        if (a.wasWhiteKing())
        {
            metadata.castle.didWhiteKingMove = true;
        }
        else if (a.wasWhiteRook() && a.row == 7)
        {
            if (a.col == 0)
            {
                metadata.castle.didWhiteQueensideRookMove = true;
            }
            else if (a.col == 7)
            {
                metadata.castle.didWhiteKingsideRookMove = true;
            }
        }

        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        return true;
    }

    // promotion
    if (a.wasWhitePawn() && (b.isWhiteBishop() || b.isWhiteKnight() || b.isWhiteRook() || b.isWhiteQueen()))
    {
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, true, b.curr, encoding);
        return true;
    }

    return false;
}

bool _isBlackEnPassant(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    // --- turn check --- //
    if (metadata.turn != Color::BLACK)
    {
        return false;
    }

    // --- setup --- //
    Change a, b, c;
    int aCount = 0, bCount = 0, cCount = 0;
    for (int i = 0; i < 3; i++)
    {
        if (changes[i].wasBlackPawn() && changes[i].isFree())
        {
            a = changes[i];
            aCount++;
        }
        if (changes[i].wasWhitePawn() && changes[i].isFree())
        {
            b = changes[i];
            bCount++;
        }
        if (changes[i].wasFree() && changes[i].isBlackPawn())  // no possibility of promotion from en passant
        {
            c = changes[i];
            cCount++;
        }
    }

    if (aCount != 1 || bCount != 1 || cCount != 1)
    {
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        return false;
    }

    // --- special checks --- //
    // no need to check rows/ columns, since these checked in canPieceAttackCell
    moveInternalToAlgebraic(
        a.prev,
        a.row, a.col,
        c.row, c.col,  // the destination square is not the square of the captured pawn!
        true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, true, false, 'x', encoding);

    return true;
}

bool _isWhiteEnPassant(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    // --- turn check --- //
    if (metadata.turn != Color::WHITE)
    {
        return false;
    }

    // --- setup --- //
    Change a, b, c;
    int aCount = 0, bCount = 0, cCount = 0;
    for (int i = 0; i < 3; i++)
    {
        if (changes[i].wasWhitePawn() && changes[i].isFree())
        {
            a = changes[i];
            aCount++;
        }
        if (changes[i].wasBlackPawn() && changes[i].isFree())
        {
            b = changes[i];
            bCount++;
        }
        if (changes[i].wasFree() && changes[i].isWhitePawn())  // no possibility of promotion from en passant
        {
            c = changes[i];
            cCount++;
        }
    }

    if (aCount != 1 || bCount != 1 || cCount != 1)
    {
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        return false;
    }

    // --- special checks --- //
    // no need to check rows/ columns, since these checked in canPieceAttackCell
    moveInternalToAlgebraic(
        a.prev,
        a.row, a.col,
        c.row, c.col,  // the destination square is not the square of the captured pawn!
        true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, true, false, 'x', encoding);

    return true;
}

bool _isBlackCastle(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    // --- turn check --- //
    if (metadata.turn != Color::BLACK)
    {
        return false;
    }

    // --- setup --- //
    Change a, b, c, d;
    int aCount = 0, bCount = 0, cCount = 0, dCount = 0;
    for (int i = 0; i < 4; i++)
    {
        if (changes[i].wasBlackKing() && changes[i].isFree())
        {
            a = changes[i];
            aCount++;
        }
        else if (changes[i].wasBlackRook() && changes[i].isFree())
        {
            b = changes[i];
            bCount++;
        }
        else if (changes[i].wasFree() && changes[i].isBlackKing())
        {
            c = changes[i];
            cCount++;
        }
        else if (changes[i].wasFree() && changes[i].isBlackRook())
        {
            d = changes[i];
            dCount++;
        }
    }

    if (aCount != 1 || bCount != 1 || cCount != 1 || dCount != 1)
    {
        return false;
    }

    // --- move check --- //
    // it is not an attacking movement, so no check

    // --- special checks --- //
    // kingside castle
    if (!metadata.castle.didBlackKingMove && !metadata.castle.didBlackKingsideRookMove &&
        a.row == 0 && a.col == 4 &&
        b.row == 0 && b.col == 7 &&
        c.row == 0 && c.col == 6 &&
        d.row == 0 && d.col == 5 &&
        !isCellInCheck(prevBoard, 0, 5, metadata),
        !isCellInCheck(prevBoard, 0, 6, metadata))
    {
        moveInternalToAlgebraic(
            'x',
            -1, -1,
            -1, -1,
            false, true, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        metadata.castle.didBlackKingMove = true;  // avoid possibility of future castling
        return true;
    }

    // queen side castle
    if (!metadata.castle.didBlackKingMove && !metadata.castle.didBlackQueensideRookMove &&
        a.row == 0 && a.col == 4 &&
        b.row == 0 && b.col == 0 &&
        c.row == 0 && c.col == 2 &&
        d.row == 0 && d.col == 3 &&
        IS_FREE(prevBoard[0][1]) &&  // no change shall be there since only 4 changes
        !isCellInCheck(prevBoard, 0, 3, metadata),
        !isCellInCheck(prevBoard, 0, 2, metadata))
    {
        moveInternalToAlgebraic(
            'x',
            -1, -1,
            -1, -1,
            false, false, true, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        metadata.castle.didBlackKingMove = true;  // avoid possibility of future castling
        return true;
    }

    return false;
}

bool _isWhiteCastle(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    // --- turn check --- //
    if (metadata.turn != Color::WHITE)
    {
        return false;
    }

    // --- setup --- //
    Change a, b, c, d;
    int aCount = 0, bCount = 0, cCount = 0, dCount = 0;
    for (int i = 0; i < 4; i++)
    {
        if (changes[i].wasWhiteKing() && changes[i].isFree())
        {
            a = changes[i];
            aCount++;
        }
        else if (changes[i].wasWhiteRook() && changes[i].isFree())
        {
            b = changes[i];
            bCount++;
        }
        else if (changes[i].wasFree() && changes[i].isWhiteKing())
        {
            c = changes[i];
            cCount++;
        }
        else if (changes[i].wasFree() && changes[i].isWhiteRook())
        {
            d = changes[i];
            dCount++;
        }
    }

    if (aCount != 1 || bCount != 1 || cCount != 1 || dCount != 1)
    {
        return false;
    }

    // --- move check --- //
    // it is not an attacking movement, so no check

    // --- special checks --- //
    // kingside castle
    if (!metadata.castle.didWhiteKingMove && !metadata.castle.didWhiteKingsideRookMove &&
        a.row == 7 && a.col == 4 &&
        b.row == 7 && b.col == 7 &&
        c.row == 7 && c.col == 6 &&
        d.row == 7 && d.col == 5 &&
        !isCellInCheck(prevBoard, 7, 5, metadata),
        !isCellInCheck(prevBoard, 7, 6, metadata))
    {
        moveInternalToAlgebraic(
            'x',
            -1, -1,
            -1, -1,
            false, true, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        metadata.castle.didWhiteKingMove = true;  // avoid possibility of future castling
        return true;
    }

    // queen side castle
    if (!metadata.castle.didWhiteKingMove && !metadata.castle.didWhiteQueensideRookMove &&
        a.row == 7 && a.col == 4 &&
        b.row == 7 && b.col == 0 &&
        c.row == 7 && c.col == 2 &&
        d.row == 7 && d.col == 3 &&
        IS_FREE(prevBoard[7][1]) &&  // no change shall be there since only 4 changes
        !isCellInCheck(prevBoard, 7, 3, metadata),
        !isCellInCheck(prevBoard, 7, 2, metadata))
    {
        moveInternalToAlgebraic(
            'x',
            -1, -1,
            -1, -1,
            false, false, true, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        metadata.castle.didWhiteKingMove = true;  // avoid possibility of future castling
        return true;
    }

    return false;
}

// Checks if prevBoard to currBoard transition can happen in one legal move.
//      If yes, message will contain S_SUCCESS (message.h) and encoding will contain the encoding of the move
//      If not, message will contain the corresponding error message (message.h) and encoding must be discarded
void processMove(char prevBoard[][8], char currBoard[][8], Metadata metadata, char message[100], char encoding[10], std::ofstream& log)
{
    // gather changed cells and needed info
    std::vector<Change> changes;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (prevBoard[i][j] != currBoard[i][j])
            {
                changes.push_back(Change{ i, j, prevBoard[i][j], currBoard[i][j] });
            }
        }
    }

    if (changes.size() == 0)
    {
        log << "No movement." << std::endl;
        strcpy(message, S_SUCCESS);
        return;
    }

    /*              TODO

    // cannot put ourselves in check / cannot leave ourselves in check
    if (isKingInCheck(currBoard, metadata.turn, metadata))
    {
        if (isKingInCheck(prevBoard, metadata.turn, metadata))
        {
            strcpy(message, E_DOES_NOT_SAVE_KING);
            return;
        }
        else
        {
            strcpy(message, E_PUTS_KING_IN_CHECK);
            return;
        }
    }

    bool isOppositeKingInCheckmate = (metadata.turn == Color::WHITE)
        ? isKingInCheckmate(currBoard, Color::BLACK, metadata)
        : isKingInCheckmate(currBoard, Color::WHITE, metadata);

    bool isOppositeKingInCheck = (metadata.turn == Color::WHITE)
        ? isKingInCheck(currBoard, Color::BLACK, metadata)
        : isKingInCheck(currBoard, Color::WHITE, metadata);
        

    if (changes.size() == 2)
    {
        if (_isBlackMove(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, encoding))
        {
            strcpy(message, S_SUCCESS);
            return;
        }
        if (_isWhiteMove(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, encoding))
        {
            strcpy(message, S_SUCCESS);
            std::cout << "In processMove: " << encoding << std::endl;
            return;
        }
        if (_isBlackCapture(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, encoding))
        {
            strcpy(message, S_SUCCESS);
            return;
        }
        if (_isWhiteCapture(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, encoding))
        {
            strcpy(message, S_SUCCESS);
            return;
        }
        strcpy(message, E_ILLEGAL_MOVE);
        return;
    }

    if (changes.size() == 3)
    {
        if (_isBlackEnPassant(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, encoding))
        {
            strcpy(message, S_SUCCESS);
            return;
        }
        if (_isWhiteEnPassant(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, encoding))
        {
            strcpy(message, S_SUCCESS);
            return;
        }
        strcpy(message, E_ILLEGAL_MOVE);
        return;
    }

    if (changes.size() == 4)
    {
        if (_isBlackCastle(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, encoding))
        {
            strcpy(message, S_SUCCESS);
            return;
        }
        if (_isWhiteCastle(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, encoding))
        {
            strcpy(message, S_SUCCESS);
            return;
        }
        strcpy(message, E_ILLEGAL_MOVE);
        return;
    }
    */
    strcpy(message, E_ILLEGAL_MOVE);
    return;
}