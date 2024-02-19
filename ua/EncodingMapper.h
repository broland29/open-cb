#pragma once

#include <iostream>
#include <QDebug>

class EncodingMapper
{
public:
    static void map(QChar input, std::string &output);
    static void map(std::string input, char &output);
};


