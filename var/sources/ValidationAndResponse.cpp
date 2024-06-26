#include "..\headers\ValidationAndResponse.h"


ValidationAndResponse::ValidationAndResponse()
{
	moveLogger = new MoveLogger();
	newGame();
}


void ValidationAndResponse::newGame()
{
	validator = new Validator();
	drawWasOffered = false;
	isGameRunning = true;
	moveLogger->newGame();
}

void ValidationAndResponse::validateMoveSlot(QVector<QString> encodings)
{
	if (!isGameRunning)
	{
		emit newGameReplySignal(false, "Game is over. Start a new game.");
		return;
	}

	char board[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			QString encoding = encodings[i * 8 + j];
			if (encoding == "FR") { board[i][j] = FR; continue; }
			if (encoding == "WP") { board[i][j] = WP; continue; }
			if (encoding == "WB") { board[i][j] = WB; continue; }
			if (encoding == "WN") { board[i][j] = WN; continue; }
			if (encoding == "WR") { board[i][j] = WR; continue; }
			if (encoding == "WQ") { board[i][j] = WQ; continue; }
			if (encoding == "WK") { board[i][j] = WK; continue; }
			if (encoding == "BP") { board[i][j] = BP; continue; }
			if (encoding == "BB") { board[i][j] = BB; continue; }
			if (encoding == "BN") { board[i][j] = BN; continue; }
			if (encoding == "BR") { board[i][j] = BR; continue; }
			if (encoding == "BQ") { board[i][j] = BQ; continue; }
			if (encoding == "BK") { board[i][j] = BK; continue; }
			else {
				SPDLOG_ERROR("Unknown encoding {}", encoding.toStdString());
				emit validateMoveReplySignal(false, "", "Internal error, encoding not recognized");
				return;
			}
		}
	}
	
	bool isValid;
	std::string encoding, description;
	validator->validateBoard(board, isValid, encoding, description);

	if (isValid)
	{
		// could check for addMove success, but not considered essential
		moveLogger->addMove(validator->getMoveCount(), encoding);

		// any correct move is a denial of a possible draw offer
		drawWasOffered = false;
	}

	emit validateMoveReplySignal(isValid, QString::fromStdString(encoding), QString::fromStdString(description));
}


void ValidationAndResponse::discardMoveSlot()
{
	QVector<QString> encodings;
	encodings.resize(64);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
#pragma warning(push)
#pragma warning(disable:26451)
			if (validator->currBoard[i][j] == FR) { encodings[i * 8 + j] = "FR"; continue; }
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
#pragma warning pop
		}									  
	}
	emit discardMoveReplySignal(true, "Discarded successfully", encodings);
}


void ValidationAndResponse::openLogsSlot()
{
	if (moveLogger->openLog() != 0)
	{
		emit openLogsReplySignal(false, "Failed to open log.");
		return;
	}
	emit openLogsReplySignal(true, "Log opened successfully.");
}


void ValidationAndResponse::newGameSlot()
{
	if (isGameRunning)
	{
		emit newGameReplySignal(false, "Cannot start new game since current game is running.");
		return;
	}

	newGame();
	emit newGameReplySignal(true, "New game started");
}


void ValidationAndResponse::surrenderSlot()
{
	if (!isGameRunning)
	{
		emit newGameReplySignal(false, "Game is over. Start a new game.");
		return;
	}

	std::string message;
	isGameRunning = false;
	moveLogger->endGame(EndGameReason::SURRENDER, validator->getLastMoveColor(), message);
	emit surrenderReplySignal(true, QString::fromStdString(message));
}


void ValidationAndResponse::offerDrawSlot()
{
	if (!isGameRunning)
	{
		emit newGameReplySignal(false, "Game is over. Start a new game.");
		return;
	}

	if (!drawWasOffered)
	{
		drawWasOffered = true;
		emit offerDrawReplySignal(true, "Draw was offered. Press the Draw button to accept, ignore to \"reject\".");
		return;
	}

	std::string message;
	isGameRunning = false;
	moveLogger->endGame(EndGameReason::INITIATED_DRAW, validator->getLastMoveColor(), message);
	emit offerDrawReplySignal(true, QString::fromStdString(message));
}


void ValidationAndResponse::test()
{
	std::cout << "Hello from ValidationAndResponse!" << std::endl;
}

