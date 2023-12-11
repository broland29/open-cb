#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "Attack.h"

#pragma warning(disable : 4996)


#define S_SUCCESS               "Correct move."

// information messages
#define I_NO_MOVE               "[INFO]  No movement."

// error messages
#define E_BAD_INITIAL_SETUP     "[ERROR] Bad initial setup."
#define E_PUTS_KING_IN_CHECK    "[ERROR] Movement puts king in check."
#define E_DOES_NOT_SAVE_KING    "[ERROR] King in check, should move it."
#define E_ILLEGAL_MOVE          "[ERROR] Illegal move."

bool _isBlackCapture(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isBlackCastle(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isBlackEnPassant(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isBlackMove(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isWhiteCapture(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isWhiteCastle(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isWhiteEnPassant(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isWhiteMove(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
void processMove(char prevBoard[][8], char currBoard[][8], Metadata metadata, char message[100], char encoding[10], std::ofstream& log);
