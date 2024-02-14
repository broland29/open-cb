#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "Attack.h"

#pragma warning(disable : 4996)


bool _isWhiteMove(
    char prevBoard[8][8],
    std::vector<Change> changes,
    Metadata metadata,
    bool isOppositeKingInCheck,
    bool isOppositeKingInCheckmate,
    char encoding[10]
);


bool _isBlackMove(
    char prevBoard[8][8],
    std::vector<Change> changes,
    Metadata metadata,
    bool isOppositeKingInCheck,
    bool isOppositeKingInCheckmate,
    char encoding[10]
);


bool _isWhiteCapture(
    char prevBoard[8][8],
    std::vector<Change> changes,
    Metadata metadata,
    bool isOppositeKingInCheck,
    bool isOppositeKingInCheckmate,
    char encoding[10]
);


bool _isBlackCapture(
    char prevBoard[8][8],
    std::vector<Change> changes,
    Metadata metadata,
    bool isOppositeKingInCheck,
    bool isOppositeKingInCheckmate,
    char encoding[10]
);


bool _isWhiteEnPassant(
    char prevBoard[8][8],
    std::vector<Change> changes,
    Metadata metadata,
    bool isOppositeKingInCheck,
    bool isOppositeKingInCheckmate,
    char encoding[10]
);


bool _isBlackEnPassant(
    char prevBoard[8][8],
    std::vector<Change> changes,
    Metadata metadata,
    bool isOppositeKingInCheck,
    bool isOppositeKingInCheckmate,
    char encoding[10]
);


bool _isWhiteCastle(
    char prevBoard[8][8],
    std::vector<Change> changes,
    Metadata metadata,
    bool isOppositeKingInCheck,
    bool isOppositeKingInCheckmate,
    char encoding[10]
);


bool _isBlackCastle(
    char prevBoard[8][8],
    std::vector<Change> changes,
    Metadata metadata,
    bool isOppositeKingInCheck,
    bool isOppositeKingInCheckmate,
    char encoding[10]
);


// Based on prevBoard, currBoard and metadata, returns through message a string containing info about the move.
//   Starting letter defines if message describes an (I)llegal or (L)egal move 
//   0  1  2  ...  199     0  1  2  ...  49  
//   I  [  message   ]
void processMove(
    char prevBoard[][8],
    char currBoard[][8],
    Metadata metadata,
    char message[200]
);
