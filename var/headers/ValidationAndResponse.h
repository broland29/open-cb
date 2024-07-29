#pragma once

#include "Validator.h"
#include "MoveLogger.h"
#include <iostream>
#include <QVector>
#include <QString>


// shall not contain pointer fields since shallow copy used in validateBoard
class ValidationAndResponse : public QObject
{
	Q_OBJECT

public:
	

private:
	Validator* validator;
	MoveLogger* moveLogger;
	bool drawWasOffered;
	bool isGameRunning;  // if the game is not over due to a GameEndReason, it shall be true

public:
	ValidationAndResponse();
	static void test();

private:
	void newGame();

signals:
	void validateMoveReplySignal(
		bool isValid,			// is the move valid or not
		QString encoding,		// if valid, the move in long algebraic notation
		QString description		// if valid, the move as categorized by VAR, if invalid, some explanation
	);
	
	void discardMoveReplySignal(
		bool succeeded,			
		QString message,			
		QVector<QString> encodings	// the board as it was before the move, so that the chess GUI can be reset
	);

	void openLogsReplySignal(
		bool succeeded,				
		QString message				
	);

	void newGameReplySignal(
		bool succeeded,		
		QString message		
	);
	
	void surrenderReplySignal(
		bool succeeded,		
		QString message	
	);

	void offerDrawReplySignal(
		bool succeded,
		QString message
	);

public slots:
	void validateMoveSlot(
		QVector<QString> encodings	// the current board encoded. previous board already stored
	);
	
	void discardMoveSlot();

	void openLogsSlot();

	void newGameSlot();
	
	void surrenderSlot();
	
	void offerDrawSlot();
};