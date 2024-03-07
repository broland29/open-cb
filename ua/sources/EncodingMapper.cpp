#include "../headers/EncodingMapper.h"

void EncodingMapper::map(QChar input, std::string &output)
{
    if      (input == '*') { output = "FR"; }
    else if (input == 'P') { output = "WP"; }
    else if (input == 'B') { output = "WB"; }
    else if (input == 'N') { output = "WN"; }
    else if (input == 'R') { output = "WR"; }
    else if (input == 'Q') { output = "WQ"; }
    else if (input == 'K') { output = "WK"; }
    else if (input == 'p') { output = "BP"; }
    else if (input == 'b') { output = "BB"; }
    else if (input == 'n') { output = "BN"; }
    else if (input == 'r') { output = "BR"; }
    else if (input == 'q') { output = "BQ"; }
    else if (input == 'k') { output = "BK"; }
    else
    {
        qDebug() << "Unknown encoding" << input;
        output = "FR";
    }
}

void EncodingMapper::map(std::string input, char &output)
{
    if      (input == "FR") { output = '*'; }
    else if (input == "WP") { output = 'P'; }
    else if (input == "WB") { output = 'B'; }
    else if (input == "WN") { output = 'N'; }
    else if (input == "WR") { output = 'R'; }
    else if (input == "WQ") { output = 'Q'; }
    else if (input == "WK") { output = 'K'; }
    else if (input == "BP") { output = 'p'; }
    else if (input == "BB") { output = 'b'; }
    else if (input == "BN") { output = 'n'; }
    else if (input == "BR") { output = 'r'; }
    else if (input == "BQ") { output = 'q'; }
    else if (input == "BK") { output = 'k'; }
    else
    {
        qDebug() << "Unknown encoding" << input;
        output = '*';
    }
}
