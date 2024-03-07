#include "../headers/Map.h"


void Map::charArrayToQString(char charArray[200], QString& qString)
{
	qString = QString::fromLatin1(charArray);
}


void Map::qStringToCharMatrix(QString qString, char charMatrix[8][8])
{
	QByteArray byteArray = qString.toLocal8Bit();
	char* data = byteArray.data();
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			charMatrix[i][j] = data[i * 8 + j];
		}
	}
}


void Map::charMatrixToQString(char charMatrix[8][8], QString& qString)
{
	qString = "";
	for (int i = 0; i < 8; i++)
	{
		QString row;
		charArrayToQString(charMatrix[i], row);
		qString.append(row);
	}
}





