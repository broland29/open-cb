#pragma once
#pragma warning(disable : 4996)  // strcpy, sprintf: This function or variable may be unsafe

#include <iostream>


namespace
{
#define FR  '*'
#define WP  'P'
#define WB  'B'
#define WN  'N'
#define WR  'R'
#define WQ  'Q'
#define WK  'K'
#define BP  'p'
#define BB  'b'
#define BN  'n'
#define BR  'r'
#define BQ  'q'
#define BK  'k'
}

// important to put macros in paranthesis to avoid accidental bugs

#define IS_FREE(enc)            (enc == FR)
#define IS_WHITE_PAWN(enc)      (enc == WP)
#define IS_WHITE_BISHOP(enc)    (enc == WB)
#define IS_WHITE_KNIGHT(enc)    (enc == WN)
#define IS_WHITE_ROOK(enc)      (enc == WR)
#define IS_WHITE_QUEEN(enc)     (enc == WQ)
#define IS_WHITE_KING(enc)      (enc == WK)
#define IS_BLACK_PAWN(enc)      (enc == BP)
#define IS_BLACK_BISHOP(enc)    (enc == BB)
#define IS_BLACK_KNIGHT(enc)    (enc == BN)
#define IS_BLACK_ROOK(enc)      (enc == BR)
#define IS_BLACK_QUEEN(enc)     (enc == BQ)
#define IS_BLACK_KING(enc)      (enc == BK)

#define IS_WHITE(enc)                       (IS_WHITE_PAWN(enc) || IS_WHITE_BISHOP(enc) || IS_WHITE_KNIGHT(enc) || IS_WHITE_ROOK(enc) || IS_WHITE_QUEEN(enc) || IS_WHITE_KING(enc))
#define IS_BLACK(enc)                       (IS_BLACK_PAWN(enc) || IS_BLACK_BISHOP(enc) || IS_BLACK_KNIGHT(enc) || IS_BLACK_ROOK(enc) || IS_BLACK_QUEEN(enc) || IS_BLACK_KING(enc))

#define IS_ATTACKABLE(currEnc, destEnc)     (IS_FREE(destEnc) || IS_WHITE(currEnc) && IS_BLACK(destEnc) || IS_BLACK(currEnc) && IS_WHITE(destEnc))
#define IS_SAME_COLOR(currEnc, destEnc)     (IS_WHITE(currEnc) && IS_WHITE(destEnc) || IS_BLACK(currEnc) && IS_BLACK(destEnc))
#define IS_NOT_FREE(enc)                    (IS_WHITE(enc) || IS_BLACK(enc))


// Example: 34 -> e5
void _cellInternalToAlgebraic(
    int row,
    int col,
    char enc[3]
);


// Example: e5 -> 34
void _cellAlgebraicToInternal(
    char enc[3],
    int& row,
    int& col
);


// In algebraic piece always uppercase
char _pieceInternalToAlgebraic(
    char enc
);


// In internal, uppercase for white, lowercase for black
char _pieceAlgebraicToInternal(
    char enc,
    bool isBlack
);


// Example: see tests
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
    char output[10]
);


// Not yet implemented
void moveAlgebraicToInternal(
    char encoding,
    int& row,
    int& col
);
