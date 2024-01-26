#include "pch.h"
#include "Move.h"
#include "Logger.h"

//#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE  // needed for macro usage, before spdlog.h!
//#include "spdlog/spdlog.h"
//#include "spdlog/sinks/stdout_color_sinks.h"
//#include "spdlog/sinks/basic_file_sink.h"


class MoveTest : public testing::Test
{
protected:
    void SetUp() override
    {
        initLogger(spdlog::level::trace);
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
}


