#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "Attack.h"

#pragma warning(disable : 4996)


bool _isBlackMove(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isWhiteMove(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isBlackCapture(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isWhiteCapture(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isBlackEnPassant(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isWhiteEnPassant(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isBlackCastle(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);
bool _isWhiteCastle(char prevBoard[8][8], std::vector<Change> changes, Metadata metadata, bool isOppositeKingInCheck, bool isOppositeKingInCheckmate, char encoding[10]);

void processMove(char prevBoard[][8], char currBoard[][8], Metadata metadata, char message[200]);
