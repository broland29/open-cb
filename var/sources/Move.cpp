#include "../headers/Move.h"


bool _isWhiteMove(char prevBoard[8][8], std::vector<Change> changes, Metadata &metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    assert(changes.size() == 2);

    // --- turn check --- //
    if (metadata.turn != Color::WHITE)
    {
        SPDLOG_TRACE("Not white turn.");
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
        SPDLOG_TRACE("Incorrect setup.");
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        SPDLOG_TRACE("Incorrect move.");
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
                metadata.castle.didWhiteQueensideRookMove = true;  // does not matter if already true
            }
            else if (a.col == 7)
            {
                metadata.castle.didWhiteKingsideRookMove = true;
            }
        }
        else if (a.wasWhitePawn() && a.row == 6 && b.row == 4)
        {
            metadata.enPassantCol = b.col;
        }

        SPDLOG_TRACE("Simple move.");
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            false, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);
        return true;
    }

    // promotion
    if (a.wasWhitePawn() && (b.isWhiteBishop() || b.isWhiteKnight() || b.isWhiteRook() || b.isWhiteQueen()))
    {
        SPDLOG_TRACE("Promotion move.");
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            false, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, true, b.curr, encoding);
        return true;
    }

    SPDLOG_TRACE("Unrecognized move.");
    return false;
}

bool _isBlackMove(char prevBoard[8][8], std::vector<Change> changes, Metadata &metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    assert(changes.size() == 2);

    // --- turn check --- //
    if (metadata.turn != Color::BLACK)
    {
        SPDLOG_TRACE("Not black turn.");
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
        SPDLOG_TRACE("Incorrect setup.");
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        SPDLOG_TRACE("Incorrect move.");
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
            metadata.enPassantCol = b.col;
        }

        SPDLOG_TRACE("Simple move.");
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
        SPDLOG_TRACE("Promotion move.");
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            false, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, true, b.curr, encoding);
        return true;
    }

    return false;
}

bool _isWhiteCapture(char prevBoard[8][8], std::vector<Change> changes, Metadata &metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    assert(changes.size() == 2);

    // --- turn check --- //
    if (metadata.turn != Color::WHITE)
    {
        SPDLOG_TRACE("Not white turn.");
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
        SPDLOG_TRACE("Incorrect setup.");
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        SPDLOG_TRACE("Incorrect move");
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

        SPDLOG_TRACE("Simple capture.");
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
        SPDLOG_TRACE("Promotion capture.");
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, true, b.curr, encoding);
        return true;
    }

    SPDLOG_TRACE("Unrecognized move.");
    return false;
}

bool _isBlackCapture(char prevBoard[8][8], std::vector<Change> changes, Metadata &metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    assert(changes.size() == 2);

    // --- turn check --- //
    if (metadata.turn != Color::BLACK)
    {
        SPDLOG_TRACE("Not black turn.");
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
        SPDLOG_TRACE("Incorrect setup.");
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        SPDLOG_TRACE("Incorrect move");
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

        SPDLOG_TRACE("Simple capture.");
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
        SPDLOG_TRACE("Promotion capture.");
        moveInternalToAlgebraic(
            a.prev,
            a.row, a.col,
            b.row, b.col,
            true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, true, b.curr, encoding);
        return true;
    }

    SPDLOG_TRACE("Unrecognized move.");
    return false;
}


bool _isWhiteEnPassant(char prevBoard[8][8], std::vector<Change> changes, Metadata &metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    assert(changes.size() == 3);

    // --- turn check --- //
    if (metadata.turn != Color::WHITE)
    {
        SPDLOG_TRACE("Not white turn.");
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
        SPDLOG_TRACE("Incorrect setup.");
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        SPDLOG_TRACE("Incorrect move.");
        return false;
    }

    // --- special checks --- //
    // no need to check rows/ columns, since these checked in canPieceAttackCell
    SPDLOG_TRACE("En passant.");
    moveInternalToAlgebraic(
        a.prev,
        a.row, a.col,
        c.row, c.col,  // the destination square is not the square of the captured pawn!
        true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, true, false, 'x', encoding);

    return true;
}


bool _isBlackEnPassant(char prevBoard[8][8], std::vector<Change> changes, Metadata &metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    assert(changes.size() == 3);

    // --- turn check --- //
    if (metadata.turn != Color::BLACK)
    {
        SPDLOG_TRACE("Not black turn");
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
        SPDLOG_TRACE("Incorrect setup.");
        return false;
    }

    // --- move check --- //
    if (!canPieceAttackCell(prevBoard, a.row, a.col, b.row, b.col, metadata.enPassantCol))
    {
        SPDLOG_TRACE("Incorrect move.");
        return false;
    }

    // --- special checks --- //
    // no need to check rows/ columns, since these checked in canPieceAttackCell
    SPDLOG_TRACE("En passant.");
    moveInternalToAlgebraic(
        a.prev,
        a.row, a.col,
        c.row, c.col,  // the destination square is not the square of the captured pawn!
        true, false, false, isOppositeKingInCheck, isOppositeKingInCheckmate, true, false, 'x', encoding);

    return true;
}

bool _isWhiteCastle(char prevBoard[8][8], std::vector<Change> changes, Metadata &metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    assert(changes.size() == 4);

    // --- turn check --- //
    if (metadata.turn != Color::WHITE)
    {
        SPDLOG_TRACE("Not white turn.");
        return false;
    }

    // --- setup --- //
    Change cWKFR, cFRWK, cWRFR, cFRWR;
    int cWKFRCount = 0, cFRWKCount = 0, cWRFRCount = 0, cFRWRCount = 0;
    for (int i = 0; i < 4; i++)
    {
        if (changes[i].wasWhiteKing() && changes[i].isFree())
        {
            cWKFR = changes[i];
            cWKFRCount++;
        }
        else if (changes[i].wasFree() && changes[i].isWhiteKing())
        {
            cFRWK = changes[i];
            cFRWKCount++;
        }
        else if (changes[i].wasWhiteRook() && changes[i].isFree())
        {
            cWRFR = changes[i];
            cWRFRCount++;
        }
        else if (changes[i].wasFree() && changes[i].isWhiteRook())
        {
            cFRWR = changes[i];
            cFRWRCount++;
        }
    }

    if (cWKFRCount != 1 || cFRWKCount != 1 || cWRFRCount != 1 || cFRWRCount != 1)
    {
        SPDLOG_TRACE("Incorrect setup.");
        return false;
    }

    // --- move check --- //
    // it is not an attacking movement, so no check

    // --- special checks --- //
    // kingside castle
    if (!metadata.castle.didWhiteKingMove && !metadata.castle.didWhiteKingsideRookMove &&
        cWKFR.row == 7 && cWKFR.col == 4 &&
        cFRWK.row == 7 && cFRWK.col == 6 &&
        cWRFR.row == 7 && cWRFR.col == 7 &&
        cFRWR.row == 7 && cFRWR.col == 5 &&
        !isCellInCheck(prevBoard, 7, 5, metadata.enPassantCol, Color::BLACK) &&
        !isCellInCheck(prevBoard, 7, 6, metadata.enPassantCol, Color::BLACK))
    {
        SPDLOG_TRACE("Kingside castle.");
        moveInternalToAlgebraic(
            'x',
            -1, -1,
            -1, -1,
            false, true, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);

        // avoid possibility of future castling
        metadata.castle.didWhiteKingMove = true;
        metadata.castle.didWhiteKingsideRookMove = true;

        return true;
    }

    // queen side castle
    if (!metadata.castle.didWhiteKingMove && !metadata.castle.didWhiteQueensideRookMove &&
        cWKFR.row == 7 && cWKFR.col == 4 &&
        cFRWK.row == 7 && cFRWK.col == 2 &&
        cWRFR.row == 7 && cWRFR.col == 0 &&
        cFRWR.row == 7 && cFRWR.col == 3 &&
        IS_FREE(prevBoard[7][1]) &&  // no change shall be there since only 4 changes
        !isCellInCheck(prevBoard, 7, 3, metadata.enPassantCol, Color::BLACK) &&
        !isCellInCheck(prevBoard, 7, 2, metadata.enPassantCol, Color::BLACK))
    {
        SPDLOG_TRACE("Queenside castle");
        moveInternalToAlgebraic(
            'x',
            -1, -1,
            -1, -1,
            false, false, true, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);

        // avoid possibility of future castling
        metadata.castle.didWhiteKingMove = true;
        metadata.castle.didWhiteQueensideRookMove = true;
        return true;
    }

    SPDLOG_TRACE("Unrecognized move.");
    return false;
}


bool _isBlackCastle(char prevBoard[8][8], std::vector<Change> changes, Metadata &metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10])
{
    assert(changes.size() == 4);

    // --- turn check --- //
    if (metadata.turn != Color::BLACK)
    {
        SPDLOG_TRACE("Not black turn.");
        return false;
    }

    // --- setup --- //
    Change cBKFR, cFRBK, cBRFR, cFRBR;
    int cBKFRCount = 0, cFRBKCount = 0, cBRFRCount = 0, cFRBRCount = 0;
    for (int i = 0; i < 4; i++)
    {
        if (changes[i].wasBlackKing() && changes[i].isFree())
        {
            cBKFR = changes[i];
            cBKFRCount++;
        }
        else if (changes[i].wasFree() && changes[i].isBlackKing())
        {
            cFRBK = changes[i];
            cFRBKCount++;
        }
        else if (changes[i].wasBlackRook() && changes[i].isFree())
        {
            cBRFR = changes[i];
            cBRFRCount++;
        }
        else if (changes[i].wasFree() && changes[i].isBlackRook())
        {
            cFRBR = changes[i];
            cFRBRCount++;
        }
    }

    if (cBKFRCount != 1 || cFRBKCount != 1 || cBRFRCount != 1 || cFRBRCount != 1)
    {
        SPDLOG_TRACE("Incorrect setup.");
        return false;
    }

    // --- move check --- //
    // it is not an attacking movement, so no check

    // --- special checks --- //
    // kingside castle
    if (!metadata.castle.didBlackKingMove && !metadata.castle.didBlackKingsideRookMove &&
        cBKFR.row == 0 && cBKFR.col == 4 &&
        cFRBK.row == 0 && cFRBK.col == 6 &&
        cBRFR.row == 0 && cBRFR.col == 7 &&
        cFRBR.row == 0 && cFRBR.col == 5 &&
        !isCellInCheck(prevBoard, 0, 5, metadata.enPassantCol, Color::WHITE) &&
        !isCellInCheck(prevBoard, 0, 6, metadata.enPassantCol, Color::WHITE))
    {
        SPDLOG_TRACE("Kingside castle.");
        moveInternalToAlgebraic(
            'x',
            -1, -1,
            -1, -1,
            false, true, false, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);

        // avoid possibility of future castling
        metadata.castle.didBlackKingMove = true;
        metadata.castle.didBlackKingsideRookMove = true;
        return true;
    }

    // queen side castle
    if (!metadata.castle.didBlackKingMove && !metadata.castle.didBlackQueensideRookMove &&
        cBKFR.row == 0 && cBKFR.col == 4 &&
        cFRBK.row == 0 && cFRBK.col == 2 &&
        cBRFR.row == 0 && cBRFR.col == 0 &&
        cFRBR.row == 0 && cFRBR.col == 3 &&
        IS_FREE(prevBoard[0][1]) &&  // no change shall be there since only 4 changes
        !isCellInCheck(prevBoard, 0, 3, metadata.enPassantCol, Color::WHITE) &&
        !isCellInCheck(prevBoard, 0, 2, metadata.enPassantCol, Color::WHITE))
    {
        SPDLOG_TRACE("Queenside castle.");
        moveInternalToAlgebraic(
            'x',
            -1, -1,
            -1, -1,
            false, false, true, isOppositeKingInCheck, isOppositeKingInCheckmate, false, false, 'x', encoding);

        // avoid possibility of future castling
        metadata.castle.didBlackKingMove = true;
        metadata.castle.didBlackQueensideRookMove = true;

        return true;
    }

    SPDLOG_TRACE("Unrecognized move.");
    return false;
}


void processMove(char prevBoard[8][8], char currBoard[8][8], Metadata& metadata, bool& isValid, std::string& encoding, std::string& description)
{
    SPDLOG_TRACE("Entered processMove");
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
        isValid = false;  // considered illegal, so that it does not affect the turns
        encoding = "";
        description = "No movement";
        return;
    }

    KingSituation prevTurnKingSituation;
    KingSituation currTurnKingSituation;
    KingSituation currOppositeKingSituation;
    if (metadata.turn == Color::WHITE)
    {
        prevTurnKingSituation = getKingSituation(prevBoard, Color::WHITE, metadata.enPassantCol);
        currTurnKingSituation = getKingSituation(currBoard, Color::WHITE, metadata.enPassantCol);
        currOppositeKingSituation = getKingSituation(currBoard, Color::BLACK, metadata.enPassantCol);
    }
    else
    {
        prevTurnKingSituation = getKingSituation(prevBoard, Color::BLACK, metadata.enPassantCol);
        currTurnKingSituation = getKingSituation(currBoard, Color::BLACK, metadata.enPassantCol);
        currOppositeKingSituation = getKingSituation(currBoard, Color::WHITE, metadata.enPassantCol);
    }


    // cannot put ourselves in check / cannot leave ourselves in check
    if (currTurnKingSituation == KingSituation::CHECKMATE)
    {
        isValid = false;
        encoding = "";
        if (prevTurnKingSituation == KingSituation::CHECK)
        {
            description = "Cannot leave yourself in check and put yourself in checkmate";
        }
        else
        {
            description = "Cannot put yourself in checkmate";
        }
        return;
    }
    if (currTurnKingSituation == KingSituation::CHECK)
    {
        isValid = false;
        encoding = "";
        if (prevTurnKingSituation == KingSituation::CHECK)
        {
            description = "Cannot leave yourself in check";
        }
        else
        {
            description = "Cannot put yourself in check";
        }
        return;
    }

    bool isOppositeKingInCheck = (currOppositeKingSituation == KingSituation::CHECK);
    bool isOppositeKingInCheckmate = (currOppositeKingSituation == KingSituation::CHECKMATE);
    char _encoding[10];

    int changesCount = changes.size();
    if (changesCount == 2)
    {
        if (_isWhiteMove(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, _encoding))
        {
            isValid = true;
            encoding = _encoding;
            description = "White move";
            return;
        }
        if (_isBlackMove(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, _encoding))
        {
            isValid = true;
            encoding = _encoding;
            description = "Black move";
            return;
        }
        if (_isWhiteCapture(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, _encoding))
        {
            isValid = true;
            encoding = _encoding;
            description = "White capture";
            return;
        }
        if (_isBlackCapture(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, _encoding))
        {
            isValid = true;
            encoding = _encoding;
            description = "Black capture";
            return;
        }

        isValid = false;
        encoding = "";
        description = "Unrecognized move (2 changes)";
        return;
    }
    else if (changesCount == 3)
    {
        if (_isWhiteEnPassant(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, _encoding))
        {
            isValid = true;
            encoding = _encoding;
            description = "White en passant";
            return;
        }
        if (_isBlackEnPassant(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, _encoding))
        {
            isValid = true;
            encoding = _encoding;
            description = "Black en passant";
            return;
        }

        isValid = false;
        encoding = "";
        description = "Unrecognized move (3 changes)";
        return;
    }
    else if (changesCount == 4)
    {
        if (_isWhiteCastle(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, _encoding))
        {
            isValid = true;
            encoding = _encoding;
            description = "White castle";
            return;
        }
        if (_isBlackCastle(prevBoard, changes, metadata, isOppositeKingInCheck, isOppositeKingInCheckmate, _encoding))
        {
            isValid = true;
            encoding = _encoding;
            description = "Black castle";
            return;
        }

        isValid = false;
        encoding = "";
        description = "Unrecognized move (4 changes)";
        return;
    }
    else
    {
        isValid = false;
        encoding = "";
        description = "Unrecognized move (" + std::to_string(changesCount) + " changes)";
        return;
    }
}
