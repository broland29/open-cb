#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "Color.h"
#include "EndGameReason.h"
#include "../com/headers/Paths.h"


class MoveLogger
{
public:

private:
	std::string currentLogPath;  // at every session, a new log is started
	int previousMoveCount;
	int gameCount;

public:
	MoveLogger();
	int newGame();
	int addMove(int moveCount, std::string move);
	int endGame(EndGameReason endGameReason, Color lastMoveColor, std::string& message);
	int openLog();

private:

};


