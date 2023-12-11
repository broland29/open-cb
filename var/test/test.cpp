#include "pch.h"
#include "Notation.h"
#include "Board.h"
#include "Attack.h"



TEST(NotationTest, moveInternalToAlgebraic) {
    char enc[10];

    // --- https://www.chess.com/terms/chess-notation --- //
    // game until pawn takes
    moveInternalToAlgebraic(WP, 6, 4, 4, 4, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "e2e4");
    moveInternalToAlgebraic(BP, 1, 4, 3, 4, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "e7e5");
    moveInternalToAlgebraic(WN, 7, 6, 5, 5, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "Ng1f3");
    moveInternalToAlgebraic(BN, 0, 1, 2, 2, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "Nb8c6");
    moveInternalToAlgebraic(WB, 7, 5, 3, 1, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "Bf1b5");
    moveInternalToAlgebraic(BP, 1, 0, 2, 0, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "a7a6");
    moveInternalToAlgebraic(WB, 3, 1, 2, 2, true, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "Bb5xc6");
    moveInternalToAlgebraic(BP, 1, 3, 2, 2, true, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "d7xc6");

    // bishop check
    moveInternalToAlgebraic(WB, 7, 5, 3, 1, false, false, false, true, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "Bf1b5+");

    // fool's mate
    moveInternalToAlgebraic(WP, 6, 5, 5, 5, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "f2f3");
    moveInternalToAlgebraic(BP, 1, 4, 3, 4, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "e7e5");
    moveInternalToAlgebraic(WP, 6, 6, 4, 6, false, false, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "g2g4");
    moveInternalToAlgebraic(BQ, 0, 3, 4, 7, false, false, false, false, true, false, false, 'x', enc);
    EXPECT_STREQ(enc, "Qd8h4#");

    // kingside castle
    moveInternalToAlgebraic('x', -1, -1, -1, -1, false, true, false, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "0-0");

    // queenside castle
    moveInternalToAlgebraic('x', -1, -1, -1, -1, false, false, true, false, false, false, false, 'x', enc);
    EXPECT_STREQ(enc, "0-0-0");

    // en passant
    moveInternalToAlgebraic(BP, 4, 1, 5, 0, true, false, false, false, false, true, false, 'x', enc);
    EXPECT_STREQ(enc, "b4xa3ep");

    // promotion without capture
    moveInternalToAlgebraic(BP, 6, 2, 7, 2, false, false, false, false, false, false, true, BQ, enc);
    EXPECT_STREQ(enc, "c2c1Q");

    // promotion with capture
    moveInternalToAlgebraic(WP, 1, 7, 0, 7, true, false, false, false, false, false, true, WN, enc);
    EXPECT_STREQ(enc, "h7xh8N");
}


TEST(BoardTest, isBoardInitialSetup) {
    char board0[8][8] =
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
    EXPECT_TRUE(isBoardInitialSetup(board0));

    char board1[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, BP, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, FR, FR, FR, FR, FR, FR, FR,
        FR, WP, WP, WP, WP, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };
    EXPECT_FALSE(isBoardInitialSetup(board1));

    char board2[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, BP, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, WP, WP, WP, WP,
        WR, WN, WB, WK, WQ, WB, WN, WR
    };
    EXPECT_FALSE(isBoardInitialSetup(board2));
}


TEST(AttackTest, _checkPath) {
    int ret;

    // cases where movement rule allows us to reach destination and there is nothing in our way
    char board0[8][8] =
    {
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, FR, BP, BP, BP, BP, BP, BP,
        BP, BP, FR, BP, BP, BP, BP, BP,
        BP, BP, BP, FR, BP, BP, BP, BP,
        BP, BP, BP, BP, FR, BP, BP, BP,
        BP, BP, BP, BP, BP, FR, BP, BP,
        BP, BP, BP, BP, BP, BP, FR, BP,
        BP, BP, BP, BP, BP, BP, BP, FR
    };
    ret = _checkPath(board0, 0, 0, 7, 7, 1, 1);
    EXPECT_EQ(ret, 0);

    char board1[8][8] =
    {
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, FR, BP, BP, BP, BP, BP,
        BP, BP, FR, BP, BP, BP, BP, BP,
        BP, BP, FR, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP
    };
    ret = _checkPath(board1, 6, 2, 2, 2, -1, 0);
    EXPECT_EQ(ret, 0);

    char board2[8][8] =
    {
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, FR, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP
    };
    ret = _checkPath(board2, 2, 1, 6, 5, 2, 2);
    EXPECT_EQ(ret, 0);

    // cases where movement rule allows us to reach destination but there is something in our way
    char board3[8][8] =
    {
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP
    };
    ret = _checkPath(board3, 0, 0, 0, 2, 0, 1);
    EXPECT_EQ(ret, 1);

    char board4[8][8] =
    {
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, FR, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP,
        BP, BP, BP, FR, BP, BP, BP, BP,
        BP, BP, BP, BP, BP, BP, BP, BP
    };
    ret = _checkPath(board4, 0, 6, 6, 3, 2, -1);
    EXPECT_EQ(ret, 1);

    char board5[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    ret = _checkPath(board5, 0, 1, 6, 7, 1, 1);
    EXPECT_EQ(ret, 1);

    // cases where there is nothing in our way but movement rule does not allow us to reach destination
    char board6[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    ret = _checkPath(board6, 4, 4, 4, 2, 0, 1);
    EXPECT_EQ(ret, 2);

    char board7[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    ret = _checkPath(board7, 0, 0, 7, 7, -1, 0);
    EXPECT_EQ(ret, 2);
}

TEST(AttackTest, _canPawnAttackCell) {
    // simple move
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WP, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board0, 1, 1, 0, 1, -1));

    char board1[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        BP, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board1, 4, 0, 5, 0, -1));

    // simple diagonal attack
    char board2[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BQ, FR, FR, FR,
        FR, FR, FR, WP, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board2, 4, 3, 3, 4, -1));

    char board3[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, BP, FR,
        FR, FR, FR, FR, FR, FR, FR, WP,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board3, 6, 7, 5, 6, -1));

    char board4[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, BP, FR, FR, FR, FR, FR,
        FR, WP, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board4, 1, 2, 2, 1, -1));

    char board5[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, BP, FR, FR, FR, FR,
        FR, FR, FR, FR, WN, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board5, 5, 3, 6, 4, -1));

    // en passant
    char board6[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board6, 5, 3, 6, 4, -1));

    char board7[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board7, 5, 3, 6, 4, -1));
}