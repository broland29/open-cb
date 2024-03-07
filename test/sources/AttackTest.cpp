#include "../headers/Test.h"
//#include "../../var/headers/Attack.h"

/*
TEST(AttackTest, _checkPath)
{
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


TEST(AttackTest, _canPawnAttackCell)
{
    // simple move possible
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

    // simple diagonal attack possible
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

    // en passant possible
    char board6[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, BP, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board6, 3, 4, 2, 3, 3));

    char board7[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, BP, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board7, 4, 1, 5, 0, 0));

    // simple move not possible
    char board8[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board8, 6, 4, 5, 4, -1));

    char board9[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, BP, FR, FR, FR, FR, FR, FR,
        FR, BN, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board9, 1, 1, 2, 1, -1));

    // diagonal move not possible since no capture
    char board10[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, WK, FR, FR,
        FR, FR, FR, FR, FR, FR, WP, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board10, 3, 6, 2, 5, -1));

    char board11[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, BP, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board11, 4, 3, 5, 4, -1));

    // en passant not possible
    char board12[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WP, BP, FR, FR, BP, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board12, 3, 1, 2, 2, 5));  // not on en passant col

    char board13[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, WP, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board13, 2, 4, 5, 5, 5));  // illegal "jump"

    // long simple move tests
    char board14[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, BP, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board14, 1, 3, 3, 3, -1));

    char board15[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, WP,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canPawnAttackCell(board15, 6, 7, 4, 7, -1));

    char board16[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WP, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board16, 1, 2, 3, 2, -1));

    char board17[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        BP, FR, FR, FR, FR, FR, FR, FR,
        WP, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board17, 6, 0, 4, 0, -1));

    char board18[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canPawnAttackCell(board18, 1, 4, 3, 4, -1));
}


TEST(AttackTest, _canBishopAttackCell)
{
    // diagonal move possible
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WB, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canBishopAttackCell(board0, 6, 0, 3, 3));

    char board1[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BB, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canBishopAttackCell(board1, 6, 4, 4, 2));

    // diagonal move not possible
    char board2[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, WB, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, BP, FR, FR,
        FR, FR, FR, FR, FR, FR, BK, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canBishopAttackCell(board2, 3, 3, 6, 6));

    char board3[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, BB, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, BB, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canBishopAttackCell(board3, 5, 1, 7, 3));

    // move not even diagonal
    char board4[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WB, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canBishopAttackCell(board4, 4, 2, 5, 2));

    char board5[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, BB,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canBishopAttackCell(board5, 2, 7, 7, 1));
}

TEST(AttackTest, _canKnightAttackCell)
{
    // good moves
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WN, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKnightAttackCell(board0, 3, 2, 1, 3));

    char board1[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, WB, FR,
        FR, FR, FR, FR, BN, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKnightAttackCell(board1, 3, 4, 2, 6));

    char board2[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WN, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKnightAttackCell(board2, 3, 2, 4, 4));

    char board3[8][8] =
    {
        FR, BN, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKnightAttackCell(board3, 0, 1, 2, 2));

    char board4[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, WN, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKnightAttackCell(board4, 2, 5, 4, 4));

    char board5[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, BN, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKnightAttackCell(board5, 4, 2, 5, 0));

    char board6[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, WN, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKnightAttackCell(board6, 5, 5, 4, 3));

    char board7[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WQ, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, BN, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKnightAttackCell(board7, 5, 3, 3, 2));

    // cell occupied
    char board8[8][8] =
    {
        WN, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WP, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canKnightAttackCell(board8, 0, 0, 2, 1));

    char board9[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, BN, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BN, FR, FR, FR
    };
    EXPECT_FALSE(_canKnightAttackCell(board9, 7, 4, 6, 2));

    // not respecting rule
    char board10[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, WN,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canKnightAttackCell(board10, 0, 7, 2, 5));

    char board11[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, BN, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canKnightAttackCell(board11, 6, 1, 7, 7));
}

TEST(AttackTest, _canRookAttackCell)
{
    // good moves
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canRookAttackCell(board0, 5, 1, 1, 1));

    char board1[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canRookAttackCell(board1, 4, 4, 4, 5));

    char board2[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, WR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, BK
    };
    EXPECT_TRUE(_canRookAttackCell(board2, 5, 7, 7, 7));

    char board3[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WB, FR, FR, BR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canRookAttackCell(board3, 4, 5, 4, 2));

    // cell occupied
    char board4[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WR, FR, WB, FR, BQ, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canRookAttackCell(board4, 2, 1, 2, 5));

    char board5[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BR, FR, BR, FR
    };
    EXPECT_FALSE(_canRookAttackCell(board5, 7, 6, 7, 0));

    // not respecting rule
    char board6[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canRookAttackCell(board6, 4, 1, 7, 2));

    char board7[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, BR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canRookAttackCell(board7, 2, 6, 6, 1));
}

TEST(AttackTest, _canQueenAttackCell)
{
    // good moves
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, WQ, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canQueenAttackCell(board0, 4, 3, 3, 3));

    char board1[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, WP,
        FR, FR, FR, FR, FR, FR, BQ, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canQueenAttackCell(board1, 2, 6, 1, 7));

    char board2[8][8] =
    {
        FR, FR, FR, WQ, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canQueenAttackCell(board2, 0, 3, 0, 7));

    char board3[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, BQ, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, WQ, FR, FR
    };
    EXPECT_TRUE(_canQueenAttackCell(board3, 3, 1, 7, 5));

    char board4[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WQ, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canQueenAttackCell(board4, 3, 4, 5, 4));

    char board5[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, BQ, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canQueenAttackCell(board5, 2, 6, 5, 3));

    char board6[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WQ, FR, FR, FR
    };
    EXPECT_TRUE(_canQueenAttackCell(board6, 7, 4, 7, 1));

    char board7[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, BQ,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canQueenAttackCell(board7, 6, 7, 3, 4));

    // cell occupied
    char board8[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WQ, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canQueenAttackCell(board8, 4, 2, 2, 4));

    char board9[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WQ, FR, WR, BR, FR, FR, FR, BQ
    };
    EXPECT_FALSE(_canQueenAttackCell(board9, 7, 7, 7, 0));

    // not respecting rule
    char board10[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, WQ, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canQueenAttackCell(board10, 4, 3, 7, 5));

    char board11[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, BQ, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canQueenAttackCell(board11, 1, 6, 0, 4));
}

TEST(AttackTest, _canKingAttackCell)
{
    // good moves
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, BK, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKingAttackCell(board0, 4, 3, 3, 3));

    char board1[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, BQ,
        FR, FR, FR, FR, FR, FR, WK, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKingAttackCell(board1, 1, 6, 0, 7));

    char board2[8][8] =
    {
        FR, FR, FR, FR, FR, BK, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKingAttackCell(board2, 0, 5, 1, 6));

    char board3[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WK, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKingAttackCell(board3, 2, 1, 3, 1));

    char board4[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, BK,
        FR, FR, FR, FR, FR, FR, WP, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKingAttackCell(board4, 2, 7, 3, 6));

    char board5[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, WK, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKingAttackCell(board5, 5, 6, 5, 5));

    char board6[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKingAttackCell(board6, 3, 4, 2, 3));

    char board7[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WK, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(_canKingAttackCell(board7, 3, 2, 2, 2));

    // cell occupied
    char board8[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, BK, BP,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canKingAttackCell(board8, 6, 6, 6, 7));

    char board9[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WQ, FR, FR, FR, FR, FR,
        FR, WK, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canKingAttackCell(board9, 6, 1, 5, 2));

    // not respecting rule
    char board10[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canKingAttackCell(board10, 2, 4, 2, 6));

    char board11[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WK, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(_canKingAttackCell(board11, 5, 2, 0, 0));
}

TEST(AttackTest, canPieceAttackCell)
{
    // https://www.chess.com/article/view/best-chess-moves#kholmov
    char board0[8][8] =
    {
        BR, FR, BB, FR, FR, BR, FR, BK,
        FR, FR, FR, FR, BQ, BP, FR, BP,
        BP, FR, FR, BP, BP, BB, FR, WQ,
        FR, FR, FR, FR, BN, WP, FR, FR,
        FR, BP, FR, WN, WP, FR, FR, FR,
        FR, FR, WN, FR, FR, FR, FR, FR,
        WP, WP, WP, FR, FR, FR, FR, WP,
        FR, FR, WK, WR, FR, WB, WR, FR
    };
    EXPECT_TRUE(canPieceAttackCell(board0, 3, 4, 1, 3, -1));
    EXPECT_TRUE(canPieceAttackCell(board0, 2, 7, 0, 5, -1));
    EXPECT_TRUE(canPieceAttackCell(board0, 2, 4, 3, 5, -1));
    EXPECT_TRUE(canPieceAttackCell(board0, 7, 2, 7, 1, -1));
    EXPECT_TRUE(canPieceAttackCell(board0, 6, 7, 4, 7, -1));
    EXPECT_FALSE(canPieceAttackCell(board0, 0, 7, 1, 7, -1));
    EXPECT_FALSE(canPieceAttackCell(board0, 4, 3, 3, 5, -1));
    EXPECT_FALSE(canPieceAttackCell(board0, 0, 2, 2, 1, -1));
    EXPECT_FALSE(canPieceAttackCell(board0, 2, 5, 4, 3, -1));
    EXPECT_FALSE(canPieceAttackCell(board0, 4, 1, 3, 1, -1));
}

TEST(AttackTest, isCellInCheck)
{
    // cell in check
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, BQ, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(isCellInCheck(board0, 5, 2, -1));

    char board1[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, BR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WK, FR, FR, FR, FR, WB
    };
    EXPECT_TRUE(isCellInCheck(board1, 7, 2, -1));

    char board2[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, BN, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(isCellInCheck(board2, 7, 6, -1));

    char board3[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, BP, FR, FR, FR, FR, FR,
        FR, WP, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_TRUE(isCellInCheck(board3, 3, 2, -1));

    // cell not in check
    char board4[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, WK,
        FR, FR, FR, FR, FR, FR, WP, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(isCellInCheck(board4, 3, 7, -1));

    char board5[8][8] =
    {
        WK, FR, FR, FR, FR, FR, FR, FR,
        FR, BR, FR, FR, FR, FR, FR, FR,
        FR, FR, BR, FR, FR, FR, FR, FR,
        FR, FR, FR, BR, FR, FR, FR, FR,
        FR, FR, FR, FR, BR, FR, FR, FR,
        FR, FR, FR, FR, FR, BR, FR, FR,
        FR, FR, FR, FR, FR, FR, BR, FR,
        FR, FR, FR, FR, FR, FR, FR, BR
    };
    EXPECT_FALSE(isCellInCheck(board5, 0, 0, -1));

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
    EXPECT_FALSE(isCellInCheck(board6, 4, 4, -1));

    char board7[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, WQ, FR, FR, FR, FR,
        FR, FR, FR, BP, FR, FR, FR, FR,
        FR, FR, FR, BK, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(isCellInCheck(board7, 5, 3, -1));

    // special cases
    // pawn cannot attack diagonally free cell
    char board8[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WP, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_FALSE(isCellInCheck(board8, 3, 2, -1));
}


TEST(AttackTest, getKingSituation)
{
    // https://www.thesprucecrafts.com/check-checkmate-and-stalemate-611546
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, WQ, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        BR, FR, FR, FR, WK, FR, FR, FR
    };
    EXPECT_EQ(getKingSituation(board0, Color::WHITE, -1), KingSituation::CHECK);

    char board1[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        BR, FR, FR, FR, FR, FR, FR, FR,
        BR, FR, FR, FR, WK, FR, FR, FR
    };
    EXPECT_EQ(getKingSituation(board1, Color::WHITE, -1), KingSituation::CHECKMATE);

    char board2[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, BQ, BK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WK, FR, FR, FR
    };
    EXPECT_EQ(getKingSituation(board2, Color::WHITE, -1), KingSituation::STALEMATE);

    // some free cases
    char board3[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, BK, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    EXPECT_EQ(getKingSituation(board3, Color::BLACK, -1), KingSituation::FREE);

    char board4[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WQ, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, BK, FR,
        FR, FR, FR, FR, BB, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WP, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, BQ, FR, FR
    };
    EXPECT_EQ(getKingSituation(board4, Color::BLACK, -1), KingSituation::FREE);
}

*/