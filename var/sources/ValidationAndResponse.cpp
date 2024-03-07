#include "..\headers\ValidationAndResponse.h"
#include "..\headers\Map.h"


ValidationAndResponse::ValidationAndResponse()
{
	validator = new Validator();
}


void ValidationAndResponse::sendToVARSlot(QString board)
{
	char _board[8][8];
	Map::qStringToCharMatrix(board, _board);
	
	char _message[200];
	validator->validateBoard(_board, _message);

	QString message;
	Map::charArrayToQString(_message, message);
	
	emit sendToVARReplySignal(message);
}


void ValidationAndResponse::getFromVARSlot()
{
	QString board;
	Map::charMatrixToQString(validator->currBoard, board);
	emit getFromVARReplySignal(board);
}


void ValidationAndResponse::newGameSlot()
{
	validator = new Validator();
	emit newGameReplySignal(true);
}


void ValidationAndResponse::test()
{
	std::cout << "Hello from ValidationAndResponse!" << std::endl;
}

