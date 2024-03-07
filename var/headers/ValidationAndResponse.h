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
	void sendToVARReplySignal(QString message);
	void getFromVARReplySignal(QString board);
	void newGameReplySignal(bool succeeded);

public slots:
	void sendToVARSlot(QString board);
	void getFromVARSlot();
	void newGameSlot();
};