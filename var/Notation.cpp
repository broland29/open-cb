#include <string>
#include <iostream>

#include "Notation.h"

#pragma warning(disable : 4996)

// Convertion from internal notation to algebraic. Example: 34 -> e5
void _cellInternalToAlgebraic(int row, int col, char enc[3])
{
    enc[0] = 'a' + col;
    enc[1] = '0' + 8 - row;
    enc[2] = '\0';
}

// Convertion from algebraic notation to internal. Example: e5 -> 34
void _cellAlgebraicToInternal(char enc[3], int& row, int& col)
{
    col = enc[0] - 'a';
    row = '0' + 8 - enc[1];
}

// Convertion from internal piece encoding to algebraic. Always uppercase
char _pieceInternalToAlgebraic(char enc)
{
    if (islower(enc))
    {
        return toupper(enc);
    }
    return enc;
}

// Convertion from algebraic piece encoding to internal. Uppercase for white, lowercase for black
char _pieceAlgebraicToInternal(char enc, bool isBlack)
{
    if (isBlack)
    {
        return tolower(enc);
    }
    return enc;
}

// Convertion from internal move encoding to algebraic
// todo - nicer solution for passing arguments
// todo - draw offer, end of game
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

    // encoding of piece
    char pieceEnc[2] = "";
    if (!(IS_WHITE_PAWN(piece) || IS_BLACK_PAWN(piece)))
    {
        pieceEnc[0] = _pieceInternalToAlgebraic(piece);
    }

    // encoding of previous cell
    char prevEnc[3];
    _cellInternalToAlgebraic(prevRow, prevCol, prevEnc);

    // encoding for capture
    char captEnc[2] = "";
    if (isCapture)
    {
        strcpy(captEnc, "x");
    }

    // encoding of current cell
    char currEnc[3];
    _cellInternalToAlgebraic(currRow, currCol, currEnc);

    // encoding for check, checkmate
    char checkEnc[2] = "";
    if (isCheckmate)  // checkmate involves check but is stronger
    {
        strcpy(checkEnc, "#");
    }
    else if (isCheck)
    {
        strcpy(checkEnc, "+");
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
        strcpy(epEnc, "ep");
    }

    // putting it together
    // max size hardcoded 10!
    sprintf(output, "%s%s%s%s%s%s%s", pieceEnc, prevEnc, captEnc, currEnc, checkEnc, promEnc, epEnc);
    // std::cout << "In moveInternalToAlgebraic: " << output << std::endl;
}

void moveAlgebraicToInternal(char encoding, int& row, int& col)
{

}