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

void EncodingMapperIP::map(std::string input, QChar& output)
{

}