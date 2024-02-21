#pragma once

#include <iostream>
#include "Board.h"
#include "Notation.h"
#include "Notation.h"
#include "Move.h"
#include "Logger.h"

class Validator
{
    // shall not contain pointer fields since shallow copy used in validateBoard
public:
    Validator();
    void validateBoard(char board[8][8], char message[200]);
    friend std::ostream& operator<<(std::ostream& os, const Validator& validator);

private:

public:
    char prevBoard[8][8];
    char currBoard[8][8];
    Metadata metadata;

private:
    char initialSetup[8][8] = {
        { BR, BN, BB, BQ, BK, BB, BN, BR },
        { BP, BP, BP, BP, BP, BP, BP, BP },
        { FR, FR, FR, FR, FR, FR, FR, FR },
        { FR, FR, FR, FR, FR, FR, FR, FR },
        { FR, FR, FR, FR, FR, FR, FR, FR },
        { FR, FR, FR, FR, FR, FR, FR, FR },
        { WP, WP, WP, WP, WP, WP, WP, WP },
        { WR, WN, WB, WQ, WK, WB, WN, WR }
    };
};