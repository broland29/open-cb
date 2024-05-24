#include "../headers/Test.h"
#include "../../var/headers/Move.h"
//#include "Logger.h"


// https://www.chess.com/terms/chess-notation#specialcases
TEST(MoveTest, WhitePawnMove)
{
    Metadata metadata;
    bool isValid;
    std::string encoding, description;
    char boardPrev[8][8] =
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
    char boardCurr[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, BP, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };
    processMove(boardPrev, boardCurr, metadata, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "e2e4");
    EXPECT_STREQ(description.c_str(), "White move");
}


TEST(MoveTest, WhiteKnightMove)
{
    Metadata metadata;
    bool isValid;
    std::string encoding, description;
    char boardPrev[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, FR, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };
    char boardCurr[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, FR, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, WN, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, FR, WR
    };
    processMove(boardPrev, boardCurr, metadata, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Ng1f3");
    EXPECT_STREQ(description.c_str(), "White move");
}


TEST(MoveTest, WhiteBishopCapture)
{
    Metadata metadata;
    bool isValid;
    std::string encoding, description;
    char boardPrev[8][8] =
    {
        BR, FR, BB, BQ, BK, BB, BN, BR,
        FR, BP, BP, BP, FR, BP, BP, BP,
        BP, FR, BN, FR, FR, FR, FR, FR,
        FR, WB, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, FR, FR, WR
    };
    char boardCurr[8][8] =
    {
        BR, FR, BB, BQ, BK, BB, BN, BR,
        FR, BP, BP, BP, FR, BP, BP, BP,
        BP, FR, WB, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, FR, FR, WR
    };
    processMove(boardPrev, boardCurr, metadata, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Bb5xc6");
    EXPECT_STREQ(description.c_str(), "White capture");
}


TEST(MoveTest, BlackPawnCapture)
{
    Metadata metadata;
    metadata.turn = Color::BLACK;
    bool isValid;
    std::string encoding, description;
    char boardPrev[8][8] =
    {
        BR, FR, BB, BQ, BK, BB, BN, BR,
        FR, BP, BP, BP, FR, BP, BP, BP,
        BP, FR, WB, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, FR, FR, WR
    };
    char boardCurr[8][8] =
    {
        BR, FR, BB, BQ, BK, BB, BN, BR,
        FR, BP, BP, FR, FR, BP, BP, BP,
        BP, FR, BP, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, FR, FR, WR
    };
    processMove(boardPrev, boardCurr, metadata, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "d7xc6");
    EXPECT_STREQ(description.c_str(), "Black capture");
}


TEST(MoveTest, WhiteBishopCheck)
{
    Metadata metadata;
    bool isValid;
    std::string encoding, description;
    char boardPrev[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, FR, FR, BP, BP, BP, BP,
        FR, FR, FR, BP, FR, FR, FR, FR,
        FR, FR, BP, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, WN, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, FR, WR
    };
    char boardCurr[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, FR, FR, BP, BP, BP, BP,
        FR, FR, FR, BP, FR, FR, FR, FR,
        FR, WB, BP, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, WN, FR, FR,
        WP, WP, WP, WP, FR, WP, WP, WP,
        WR, WN, WB, WQ, WK, FR, FR, WR
    };
    processMove(boardPrev, boardCurr, metadata, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Bf1b5+");
    EXPECT_STREQ(description.c_str(), "White move");
}


TEST(MoveTest, BlackQueenCheckmate)
{
    Metadata metadata;
    metadata.turn = Color::BLACK;
    bool isValid;
    std::string encoding, description;
    char boardPrev[8][8] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, FR, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, WP, FR,
        FR, FR, FR, FR, FR, WP, FR, FR,
        WP, WP, WP, WP, WP, FR, FR, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };
    char boardCurr[8][8] =
    {
        BR, BN, BB, FR, BK, BB, BN, BR,
        BP, BP, BP, BP, FR, BP, BP, BP,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, WP, BQ,
        FR, FR, FR, FR, FR, WP, FR, FR,
        WP, WP, WP, WP, WP, FR, FR, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };
    processMove(boardPrev, boardCurr, metadata, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "Qd8h4#");
    EXPECT_STREQ(description.c_str(), "Black move");
}


TEST(MoveTest, WhitePromotion)
{
    Metadata metadata;
    bool isValid;
    std::string encoding, description;
    char boardPrev[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        BQ, BK, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WK, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, WQ, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    char boardCurr[8][8] =
    {
        FR, FR, FR, FR, WQ, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        BQ, BK, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WK, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, WQ, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    processMove(boardPrev, boardCurr, metadata, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "e7e8Q");
    EXPECT_STREQ(description.c_str(), "White move");
}


TEST(MoveTest, WhiteEnPassant)
{
    Metadata metadata;
    metadata.enPassantCol = 4;
    bool isValid;
    std::string encoding, description;
    char boardPrev[8][8] =
    {
        FR, FR, FR, FR, BK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, BP, WP, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WK, FR, FR, FR
    };
    char boardCurr[8][8] =
    {
        FR, FR, FR, FR, BK, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WP, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, WK, FR, FR, FR
    };
    processMove(boardPrev, boardCurr, metadata, isValid, encoding, description);
    EXPECT_TRUE(isValid);
    EXPECT_STREQ(encoding.c_str(), "f5e6ep");
    EXPECT_STREQ(description.c_str(), "White move");
}