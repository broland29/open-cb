#pragma once

#include "Validator.h"
#include <iostream>


// shall not contain pointer fields since shallow copy used in validateBoard
class ValidationAndResponse : public QObject
{
	Q_OBJECT
public:
	ValidationAndResponse();
	static void test();

private:
	Validator* validator;

signals:
	void validateMoveReplySignal(bool succeeded, QString message);
	void discardMoveReplySignal(bool succeeded, QString message);
	void newGameReplySignal(bool succeeded, QString message);

public slots:
	void validateMoveSlot(QString board);
	void discardMoveSlot();
	void newGameSlot();
};