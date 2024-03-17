#pragma once

#include <QChar>
#include <string>
#include <spdlog/spdlog.h>

class EncodingMapperIP
{
public:
    static int map(QChar input, int row, int col, std::string& output);
    static void map(std::string input, QChar& output);
};


