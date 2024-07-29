#include "../headers/Test.h"
#include "../../var/headers/Validator.h"


TEST(ValidatorTest, BadMovementsTest)
{
    Validator validator;

    // no movement
    bool isValid;
    std::string encoding, description;
    char board[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, BP, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, WP, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_FALSE(isValid);
    EXPECT_STREQ(encoding.c_str(), "");
    EXPECT_STREQ(description.c_str(), "No movement");
    EXPECT_EQ(validator.metadata.turn, Color::WHITE);
    EXPECT_EQ(validator.metadata.moveCount, 0);
    EXPECT_EQ(validator.metadata.enPassantCol, -1);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteQueensideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackQueensideRookMove);

    // just add a random pawn
    board[3][3] = BP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_FALSE(isValid);
    EXPECT_STREQ(encoding.c_str(), "");
    EXPECT_STREQ(description.c_str(), "Unrecognized move (1 changes)");
    EXPECT_EQ(validator.metadata.turn, Color::WHITE);
    EXPECT_EQ(validator.metadata.moveCount, 0);
    EXPECT_EQ(validator.metadata.enPassantCol, -1);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteQueensideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackQueensideRookMove);

    // just add a random queen
    board[4][4] = WQ;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_FALSE(isValid);
    EXPECT_STREQ(encoding.c_str(), "");
    EXPECT_STREQ(description.c_str(), "Unrecognized move (2 changes)");
    EXPECT_EQ(validator.metadata.turn, Color::WHITE);
    EXPECT_EQ(validator.metadata.moveCount, 0);
    EXPECT_EQ(validator.metadata.enPassantCol, -1);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteQueensideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackQueensideRookMove);

    // just add a random rook
    board[5][5] = BR;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_FALSE(isValid);
    EXPECT_STREQ(encoding.c_str(), "");
    EXPECT_STREQ(description.c_str(), "Unrecognized move (3 changes)");
    EXPECT_EQ(validator.metadata.turn, Color::WHITE);
    EXPECT_EQ(validator.metadata.moveCount, 0);
    EXPECT_EQ(validator.metadata.enPassantCol, -1);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteQueensideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackQueensideRookMove);
}


// https://www.chess.com/article/view/the-best-chess-games-of-all-time#Kasparov_Topalov
TEST(ValidatorTest, KasparovVSTopalov)
{
    Validator validator;
    bool isValid;
    std::string encoding, description;
    char board[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, BP, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, WP, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };

    // 1. e4 d6
    board[6][4] = FR;
    board[4][4] = WP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "e2e4");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 1);
    EXPECT_EQ(validator.metadata.enPassantCol, 4);

    board[1][3] = FR;
    board[2][3] = BP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "d7d6");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 1);
    EXPECT_EQ(validator.metadata.enPassantCol, -1);

    // 2. d4 Nf6
    board[6][3] = FR;
    board[4][3] = WP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "d2d4");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 2);
    EXPECT_EQ(validator.metadata.enPassantCol, 3);

    board[0][6] = FR;
    board[2][5] = BN;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Ng8f6");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 2);
    EXPECT_EQ(validator.metadata.enPassantCol, -1);

    // 3. Nc3 g6
    board[7][1] = FR;
    board[5][2] = WN;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Nb1c3");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 3);

    board[1][6] = FR;
    board[2][6] = BP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "g7g6");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 3);

    // 4. Be3 Bg7
    board[7][2] = FR;
    board[5][4] = WB;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Bc1e3");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 4);

    board[0][5] = FR;
    board[1][6] = BB;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Bf8g7");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 4);

    // 5. Qd2 c6
    board[7][3] = FR;
    board[6][3] = WQ;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Qd1d2");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 5);

    board[1][2] = FR;
    board[2][2] = BP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "c7c6");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 5);

    // 6. f3 b5
    board[6][5] = FR;
    board[5][5] = WP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "f2f3");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 6);

    board[1][1] = FR;
    board[3][1] = BP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "b7b5");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 6);
    EXPECT_EQ(validator.metadata.enPassantCol, 1);

    // 7. Nge2 Nbd7
    board[7][6] = FR;
    board[6][4] = WN;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Ng1e2");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 7);
    EXPECT_EQ(validator.metadata.enPassantCol, -1);

    board[0][1] = FR;
    board[1][3] = BN;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Nb8d7");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 7);

    // 8. Bh6 Bxh6
    board[5][4] = FR;
    board[2][7] = WB;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Be3h6");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 8);

    board[1][6] = FR;
    board[2][7] = BB;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Bg7xh6");
    EXPECT_STREQ(description.c_str(), "Black capture");
    EXPECT_EQ(validator.metadata.moveCount, 8);

    // 9. Qxh6 Bb7
    board[6][3] = FR;
    board[2][7] = WQ;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Qd2xh6");
    EXPECT_STREQ(description.c_str(), "White capture");
    EXPECT_EQ(validator.metadata.moveCount, 9);

    board[0][2] = FR;
    board[1][1] = BB;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Bc8b7");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 9);

    // 10. a3 e5
    board[6][0] = FR;
    board[5][0] = WP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "a2a3");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 10);

    board[1][4] = FR;
    board[3][4] = BP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "e7e5");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 10);

    // 11. O-O-O Qe7
    board[7][4] = FR;
    board[7][2] = WK;
    board[7][0] = FR;
    board[7][3] = WR;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "0-0-0");
    EXPECT_STREQ(description.c_str(), "White castle");
    EXPECT_EQ(validator.metadata.moveCount, 11);
    EXPECT_TRUE(validator.metadata.castle.didWhiteKingMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingsideRookMove);
    EXPECT_TRUE(validator.metadata.castle.didWhiteQueensideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackQueensideRookMove);

    board[0][3] = FR;
    board[1][4] = BQ;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Qd8e7");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 11);
    EXPECT_TRUE(validator.metadata.castle.didWhiteKingMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingsideRookMove);
    EXPECT_TRUE(validator.metadata.castle.didWhiteQueensideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackQueensideRookMove);

    // 12. Kb1 a6
    board[7][2] = FR;
    board[7][1] = WK;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Kc1b1");
    EXPECT_STREQ(description.c_str(), "White move");
    EXPECT_EQ(validator.metadata.moveCount, 12);
    EXPECT_TRUE(validator.metadata.castle.didWhiteKingMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingsideRookMove);
    EXPECT_TRUE(validator.metadata.castle.didWhiteQueensideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackQueensideRookMove);

    board[1][0] = FR;
    board[2][0] = BP;
    validator.validateBoard(board, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "a7a6");
    EXPECT_STREQ(description.c_str(), "Black move");
    EXPECT_EQ(validator.metadata.moveCount, 12);
    EXPECT_TRUE(validator.metadata.castle.didWhiteKingMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingMove);
    EXPECT_FALSE(validator.metadata.castle.didWhiteKingsideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackKingsideRookMove);
    EXPECT_TRUE(validator.metadata.castle.didWhiteQueensideRookMove);
    EXPECT_FALSE(validator.metadata.castle.didBlackQueensideRookMove);
}