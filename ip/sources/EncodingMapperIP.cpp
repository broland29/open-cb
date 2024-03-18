#include "../headers/EncodingMapperIP.h"

int EncodingMapperIP::map(QChar input, int row, int col, std::string& output)
{
	if (input == '*')
	{
		if ((row + col) % 2 == 0)	// white cell -> white free
		{
			output = "WF";
		}
		else						// black cell -> black free
		{
			output = "BF";
		}
	}
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
		SPDLOG_ERROR("Unknown encoding {}!", input.toLatin1());
		return 1;
	}

	return 0;
}

int EncodingMapperIP::map(std::string input, QChar& output)
{
	if (input == "WF" || input == "BF")
	{
		output = '*';  // "compress" into one category
	}
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
		SPDLOG_ERROR("Unknown encoding {}!", input);
		return 1;
	}

	return 0;
}

int EncodingMapperIP::map(std::string input, uchar& output)
{
	if (input == "WF") { output = 0; }
	else if (input == "WP") { output = 1; }
	else if (input == "WB") { output = 2; }
	else if (input == "WN") { output = 3; }
	else if (input == "WR") { output = 4; }
	else if (input == "WQ") { output = 5; }
	else if (input == "WK") { output = 6; }
	else if (input == "BF") { output = 7; }
	else if (input == "BP") { output = 8; }
	else if (input == "BB") { output = 9; }
	else if (input == "BN") { output = 10; }
	else if (input == "BR") { output = 11; }
	else if (input == "BQ") { output = 12; }
	else if (input == "BK") { output = 13; }
	else
	{
		SPDLOG_ERROR("Unknown encoding {}!", input);
		return 1;
	}

	return 0;
}

int EncodingMapperIP::map(uchar input, std::string& output)
{
	if (input == 0) { output = "WF"; }
	else if (input == 1) { output = "WP"; }
	else if (input == 2) { output = "WB"; }
	else if (input == 3) { output = "WN"; }
	else if (input == 4) { output = "WR"; }
	else if (input == 5) { output = "WQ"; }
	else if (input == 6) { output = "WK"; }
	else if (input == 7) { output = "BF"; }
	else if (input == 8) { output = "BP"; }
	else if (input == 9) { output = "BB"; }
	else if (input == 10) { output = "BN"; }
	else if (input == 11) { output = "BR"; }
	else if (input == 12) { output = "BQ"; }
	else if (input == 13) { output = "BK"; }
	else
	{
		SPDLOG_ERROR("Unknown encoding {}!", input);
		return 1;
	}

	return 0;
}