#include "..\headers\ValidationAndResponse.h"


ValidationAndResponse::ValidationAndResponse()
{
	validator = new Validator();
}


void ValidationAndResponse::validateMoveSlot(QVector<QString> encodings)
{
	char board[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			QString encoding = encodings[i * 8 + j];
			if (encoding == "WF") { board[i][j] = FR; continue; }
			if (encoding == "WP") { board[i][j] = WP; continue; }
			if (encoding == "WB") { board[i][j] = WB; continue; }
			if (encoding == "WN") { board[i][j] = WN; continue; }
			if (encoding == "WR") { board[i][j] = WR; continue; }
			if (encoding == "WQ") { board[i][j] = WQ; continue; }
			if (encoding == "WK") { board[i][j] = WK; continue; }
			if (encoding == "BF") { board[i][j] = FR; continue; }
			if (encoding == "BP") { board[i][j] = BP; continue; }
			if (encoding == "BB") { board[i][j] = BB; continue; }
			if (encoding == "BN") { board[i][j] = BN; continue; }
			if (encoding == "BR") { board[i][j] = BR; continue; }
			if (encoding == "BQ") { board[i][j] = BQ; continue; }
			if (encoding == "BK") { board[i][j] = BK; continue; }
			else {
				SPDLOG_ERROR("Unknown encoding {}", encoding.toStdString());
				emit validateMoveReplySignal(false, "Internal error");
				return;
			}
		}
	}
	
	char message[200];
	validator->validateBoard(board, message);

	emit validateMoveReplySignal(true, QString::fromLatin1(message));
}


void ValidationAndResponse::discardMoveSlot()
{
	QVector<QString> encodings;
	encodings.resize(64);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (validator->currBoard[i][j] == FR && (i + j) % 2 == 0) { encodings[i * 8 + j] = "WF"; continue; }
			if (validator->currBoard[i][j] == FR && (i + j) % 2 == 1) { encodings[i * 8 + j] = "BF"; continue; }
			if (validator->currBoard[i][j] == WP) { encodings[i * 8 + j] = "WP"; continue; }
			if (validator->currBoard[i][j] == WB) { encodings[i * 8 + j] = "WB"; continue; }
			if (validator->currBoard[i][j] == WN) { encodings[i * 8 + j] = "WN"; continue; }
			if (validator->currBoard[i][j] == WR) { encodings[i * 8 + j] = "WR"; continue; }
			if (validator->currBoard[i][j] == WQ) { encodings[i * 8 + j] = "WQ"; continue; }
			if (validator->currBoard[i][j] == WK) { encodings[i * 8 + j] = "WK"; continue; }
			if (validator->currBoard[i][j] == BP) { encodings[i * 8 + j] = "BP"; continue; }
			if (validator->currBoard[i][j] == BB) { encodings[i * 8 + j] = "BB"; continue; }
			if (validator->currBoard[i][j] == BN) { encodings[i * 8 + j] = "BN"; continue; }
			if (validator->currBoard[i][j] == BR) { encodings[i * 8 + j] = "BR"; continue; }
			if (validator->currBoard[i][j] == BQ) { encodings[i * 8 + j] = "BQ"; continue; }
			if (validator->currBoard[i][j] == BK) { encodings[i * 8 + j] = "BK"; continue; }
		}									  
	}
	emit discardMoveReplySignal(true, "Discarded successfully", encodings);
}


void ValidationAndResponse::newGameSlot()
{
	validator = new Validator();
	emit newGameReplySignal(true, "New game started");
}


void ValidationAndResponse::test()
{
	std::cout << "Hello from ValidationAndResponse!" << std::endl;
}

