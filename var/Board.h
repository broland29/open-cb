#pragma once
#include "Notation.h"

#define INITIAL_SETUP  {BR, BN, BB, BQ, BK, BB, BN, BR, \
                        BP, BP, BP, BP, BP, BP, BP, BP, \
                        FR, FR, FR, FR, FR, FR, FR, FR, \
                        FR, FR, FR, FR, FR, FR, FR, FR, \
                        FR, FR, FR, FR, FR, FR, FR, FR, \
                        FR, FR, FR, FR, FR, FR, FR, FR, \
                        WP, WP, WP, WP, WP, WP, WP, WP, \
                        WR, WN, WB, WQ, WK, WB, WN, WR}

enum class Color
{
    WHITE,
    BLACK
};

// everything related to castling
struct Castle
{
    bool didBlackKingMove = false;
    bool didWhiteKingMove = false;
    bool didBlackKingsideRookMove = false;
    bool didBlackQueensideRookMove = false;
    bool didWhiteKingsideRookMove = false;
    bool didWhiteQueensideRookMove = false;
};

// Information that propagates from one turn to another
struct Metadata
{
    Color turn = Color::WHITE;
    int moveCount = 1;

    int enPassantCol;  // -1 if en passant not possible, 0-7 if en passant possible on given column
    Castle castle;

    void changeTurn()
    {
        if (turn == Color::WHITE)
        {
            turn = Color::BLACK;
        }
        else
        {
            turn = Color::WHITE;
        }
    }
};

// Change happening in a cell - row and column stays the same since we talk about one cell
struct Change
{
    int row;
    int col;
    char prev;  // previous piece encoding
    char curr;  // current piece encoding

    // inline functions to wrap macros for better readability when working with Change
    bool inline wasBlack() { return IS_BLACK(prev); }
    bool inline wasWhite() { return IS_WHITE(prev); }
    bool inline wasFree() { return IS_FREE(prev); }
    bool inline wasWhitePawn() { return IS_WHITE_PAWN(prev); }
    bool inline wasBlackPawn() { return IS_BLACK_PAWN(prev); }
    bool inline wasWhiteBishop() { return IS_WHITE_BISHOP(prev); }
    bool inline wasBlackBishop() { return IS_BLACK_BISHOP(prev); }
    bool inline wasWhiteKnight() { return IS_WHITE_KNIGHT(prev); }
    bool inline wasBlackKnight() { return IS_BLACK_KNIGHT(prev); }
    bool inline wasWhiteRook() { return IS_WHITE_ROOK(prev); }
    bool inline wasBlackRook() { return IS_BLACK_ROOK(prev); }
    bool inline wasWhiteQueen() { return IS_WHITE_QUEEN(prev); }
    bool inline wasBlackQueen() { return IS_BLACK_QUEEN(prev); }
    bool inline wasWhiteKing() { return IS_WHITE_KING(prev); }
    bool inline wasBlackKing() { return IS_BLACK_KING(prev); }
    bool inline isBlack() { return IS_BLACK(curr); }
    bool inline isWhite() { return IS_WHITE(curr); }
    bool inline isFree() { return IS_FREE(curr); }
    bool inline isWhitePawn() { return IS_WHITE_PAWN(curr); }
    bool inline isBlackPawn() { return IS_BLACK_PAWN(curr); }
    bool inline isWhiteBishop() { return IS_WHITE_BISHOP(curr); }
    bool inline isBlackBishop() { return IS_BLACK_BISHOP(curr); }
    bool inline isWhiteKnight() { return IS_WHITE_KNIGHT(curr); }
    bool inline isBlackKnight() { return IS_BLACK_KNIGHT(curr); }
    bool inline isWhiteRook() { return IS_WHITE_ROOK(curr); }
    bool inline isBlackRook() { return IS_BLACK_ROOK(curr); }
    bool inline isWhiteQueen() { return IS_WHITE_QUEEN(curr); }
    bool inline isBlackQueen() { return IS_BLACK_QUEEN(curr); }
    bool inline isWhiteKing() { return IS_WHITE_KING(curr); }
    bool inline isBlackKing() { return IS_BLACK_KING(curr); }
};


void copyBoard(
    char boardTo[8][8],
    char boardFrom[8][8]
);


bool isBoardInitialSetup(
    char board[8][8]
);


void printBoard(
    char board[8][8],
    std::ofstream& ofstream
);


void readBoard(
    char board[8][8],
    std::ifstream& ifstream
);


bool isBoardInitialSetup(
    char board[8][8]
);