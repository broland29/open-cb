#pragma once

#include <iostream>
#include "Board.h"
#include "Notation.h"
#include "Notation.h"
#include "Move.h"

struct Validator
{
    // shall not contain pointer fields since shallow copy used in validateBoard

    char prevBoard[8][8];
    char currBoard[8][8];
    Metadata metadata;

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

    Validator();

    void validateBoard(char board[8][8], char message[200]);

    friend std::ostream& operator<<(std::ostream& os, const Validator& validator);
};