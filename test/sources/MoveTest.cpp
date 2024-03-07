#include "../headers/Test.h"
//#include "../../var/headers/Move.h"
//#include "Logger.h"



// (1) https://www.ragchess.com/how-to-checkmate-your-opponent/

/*

class MoveTest : public testing::Test
{
protected:
    void SetUp() override
    {
        //initLogger(spdlog::level::trace);
    }
};

TEST_F(MoveTest, _isWhiteMove)
{
    // long pawn move
    Metadata metadata0{ Color::WHITE, 1, -1, Castle{false, false, false, false, false, false} };
    char board0[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WP, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    std::vector<Change> changes0;
    changes0.push_back(Change{ 6, 1, WP, FR });
    changes0.push_back(Change{ 4, 1, FR, WP });
    char enc0[10];
    EXPECT_TRUE(_isWhiteMove(board0, changes0, metadata0, false, false, enc0));
    EXPECT_STREQ(enc0, "b2b4");

    // knight move
    Metadata metadata1{ Color::WHITE, 1, -1, Castle{false, false, false, false, false, false} };
    char board1[8][8] =
    {
        WK, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, BP, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, WN, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, WR, FR, FR, FR, FR, FR, FR
    };
    std::vector<Change> changes1;
    changes1.push_back(Change{ 4, 3, WN, FR });
    changes1.push_back(Change{ 2, 4, FR, WN });
    char enc1[10];
    EXPECT_TRUE(_isWhiteMove(board1, changes1, metadata1, false, false, enc1));
    EXPECT_STREQ(enc1, "Nd4e6");

    // queen captures queen - should not be recognized, since white capture, not white move
    Metadata metadata2{ Color::WHITE, 1, -1, Castle{false, false, false, false, false, false} };
    char board2[8][8] =
    {
        FR, FR, WQ, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, BQ, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    std::vector<Change> changes2;
    changes2.push_back(Change{ 0, 2, WQ, FR });
    changes2.push_back(Change{ 3, 5, BQ, WQ });
    char enc2[10];
    EXPECT_FALSE(_isWhiteMove(board2, changes2, metadata2, false, false, enc2));

    // promotion to queen
    Metadata metadata3{ Color::WHITE, 1, -1, Castle{false, false, false, false, false, false} };
    char board3[8][8] =
    {
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, WP, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR,
        FR, FR, FR, FR, FR, FR, FR, FR
    };
    std::vector<Change> changes3;
    changes3.push_back(Change{ 1, 6, WP, FR });
    changes3.push_back(Change{ 0, 6, FR, WQ });
    char enc3[10];
    EXPECT_TRUE(_isWhiteMove(board3, changes3, metadata3, false, false, enc3));
    EXPECT_STREQ(enc3, "g7g8Q");

    // (1) pawn delivering check
    Metadata metadata4{ Color::WHITE, 1, -1, Castle{true, true, true, false, true, false} };
    char board4[8][8] =
    {
        BR, FR, BB, FR, FR, FR, FR, BK,
        BP, BP, BP, BP, FR, FR, FR, BP,
        FR, FR, BN, FR, BQ, WQ, WP, FR,
        FR, FR, BB, FR, BP, FR, FR, FR,
        FR, FR, WB, FR, WP, FR, FR, WN,
        FR, FR, WN, WP, FR, BP, FR, WP,
        WP, WP, WP, WB, WR, BR, FR, FR,
        WR, FR, FR, FR, FR, FR, FR, WK
    };
    std::vector<Change> changes4;
    changes4.push_back(Change{ 2, 6, WP, FR });
    changes4.push_back(Change{ 1, 6, FR, WP });
    char enc4[10];
    EXPECT_TRUE(_isWhiteMove(board4, changes4, metadata4, true, false, enc4));
    EXPECT_STREQ(enc4, "g6g7+");
}

*/
