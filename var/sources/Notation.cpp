#include "../headers/Notation.h"


void _cellInternalToAlgebraic(int row, int col, char enc[3])
{
    enc[0] = 'a' + col;
    enc[1] = '0' + 8 - row;
    enc[2] = '\0';
}


void _cellAlgebraicToInternal(char enc[3], int& row, int& col)
{
    col = enc[0] - 'a';
    row = '0' + 8 - enc[1];
}


char _pieceInternalToAlgebraic(char enc)
{
    if (islower(enc))
    {
        return toupper(enc);
    }
    return enc;
}


char _pieceAlgebraicToInternal(char enc, bool isBlack)
{
    if (isBlack)
    {
        return tolower(enc);
    }
    return enc;
}


// todo: draw offer, end of game
void moveInternalToAlgebraic(
    char piece,
    int prevRow,
    int prevCol,
    int currRow,
    int currCol,
    bool isCapture,
    bool isKingSideCastle,
    bool isQueenSideCastle,
    bool isCheck,
    bool isCheckmate,
    bool isEnPassant,
    bool isPromotion,
    char piecePromotedTo,
    char output[10])
{
    // hard-coded encoding of special cases
    if (isKingSideCastle)
    {
        strcpy(output, "0-0");
        return;
    }
    if (isQueenSideCastle)
    {
        strcpy(output, "0-0-0");
        return;
    }

    // encoding of piece - pawns not noted
    char pieceEnc[2] = "";
    if (!(IS_WHITE_PAWN(piece) || IS_BLACK_PAWN(piece)))
    {
        pieceEnc[0] = _pieceInternalToAlgebraic(piece);
        pieceEnc[1] = '\0';
    }

    // encoding of previous cell
    char prevEnc[3];
    _cellInternalToAlgebraic(prevRow, prevCol, prevEnc);

    // encoding for capture
    char captEnc[2] = "";
    if (isCapture)
    {
        captEnc[0] = 'x';
        captEnc[1] = '\0';
    }

    // encoding of current cell
    char currEnc[3];
    _cellInternalToAlgebraic(currRow, currCol, currEnc);

    // encoding for check, checkmate
    char checkEnc[2] = "";
    if (isCheckmate)  // checkmate involves check but is stronger
    {
        checkEnc[0] = '#';
        checkEnc[1] = '\0';
    }
    else if (isCheck)
    {
        checkEnc[0] = '+';
        checkEnc[1] = '\0';
    }

    // encoding of promotion
    char promEnc[2] = "";
    if (isPromotion)
    {
        promEnc[0] = _pieceInternalToAlgebraic(piecePromotedTo);
        promEnc[1] = '\0';
    }

    // encoding of en passant
    char epEnc[3] = "";
    if (isEnPassant)
    {
        epEnc[0] = 'e';
        epEnc[1] = 'p';
        epEnc[2] = '\0';
    }

    // putting it together
    // max size hardcoded 10!
    sprintf(output, "%s%s%s%s%s%s%s", pieceEnc, prevEnc, captEnc, currEnc, checkEnc, promEnc, epEnc);
}


void moveAlgebraicToInternal(char encoding, int& row, int& col)
{
    // todo: implement if will be needed (example: user gives a command in algebraic notation)
}