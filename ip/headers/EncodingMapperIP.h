#pragma once

#include <QChar>
#include <string>
#include <spdlog/spdlog.h>

class EncodingMapperIP
{
public:
    // * P B N R Q K p b n r q k -> WF BF WP WB WN WR WQ WK BP BB BN BR BQ BK
    static int map(QChar input, int row, int col, std::string& output);

    // WF BF WP WB WN WR WQ WK BP BB BN BR BQ BK -> * P B N R Q K p b n r q k
    static int map(std::string input, QChar& output);
    
    //  WF BF WP WB WN WR WQ WK BP BB BN BR BQ BK -> 0 1 2 3 4 5 6 7 8 9 10 11 12 13
    static int map(std::string input, uchar& output);

    //  0 1 2 3 4 5 6 7 8 9 10 11 12 13 -> WF BF WP WB WN WR WQ WK BP BB BN BR BQ BK
    static int map(uchar input, std::string& output);
};


