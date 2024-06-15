#include "../headers/MoveLogger.h"

const std::string VAR_LOG_PATH = "C:\\open-cb\\mem\\var";

MoveLogger::MoveLogger()
{
	// https://stackoverflow.com/questions/9089842/c-chrono-system-time-in-milliseconds-time-operations
	std::string now = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	currentLogPath = VAR_LOG_PATH + "\\log" + now + ".txt";
	previousMoveCount = 0;
	gameCount = 0;
}


int MoveLogger::newGame()
{
	std::ofstream outfile(currentLogPath, std::ios_base::app);
	if (!outfile.is_open())
	{
		SPDLOG_ERROR("Could not open {}", currentLogPath);
		return 1;
	}

	previousMoveCount = 0;
	gameCount++;

	outfile << "Game " << std::to_string(gameCount) << std::endl;
	outfile.close();

	SPDLOG_TRACE("Start of game {} added to log", gameCount);

	return 0;
}

int MoveLogger::addMove(int moveCount, std::string move)
{
	std::ofstream outfile(currentLogPath, std::ios_base::app);
	if (!outfile.is_open())
	{
		SPDLOG_ERROR("Could not open {}", currentLogPath);
		return 1;
	}

	// two moves in one row/ per moveCount
	if (previousMoveCount != moveCount)  // start of new row
	{
		outfile << std::endl << moveCount << ": " << move;
	}
	else  // end of old row
	{
		outfile << " " << move;
	}

	previousMoveCount = moveCount;

	SPDLOG_TRACE("Move {} added to log", move);
	outfile.close();
	return 0;
}


int MoveLogger::endGame(EndGameReason endGameReason, Color lastMoveColor, std::string& message)
{
	std::ofstream outfile(currentLogPath, std::ios_base::app);
	if (!outfile.is_open())
	{
		SPDLOG_ERROR("Could not open {}", currentLogPath);
		return 1;
	}

	std::string playerOne = (lastMoveColor == Color::WHITE) ? "white" : "black";
	std::string playerTwo = (lastMoveColor == Color::WHITE) ? "black" : "white";
	
	if (endGameReason == EndGameReason::SURRENDER)
	{
		message = "Game ended: " + playerTwo + " won, " + playerOne + " surrendered.";
	}
	else if (endGameReason == EndGameReason::INITIATED_DRAW)
	{
		message = "Game ended in draw: " + playerOne + " offered, " + playerTwo + " accepted.";
	}

	outfile << std::endl << message << std::endl << "--------------------------------------------------" << std::endl;

	SPDLOG_TRACE("Logging end of game {}", gameCount);
	outfile.close();
	return 0;
}


int MoveLogger::openLog()
{
	return system(currentLogPath.c_str());
}