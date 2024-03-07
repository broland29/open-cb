#pragma once

#include <QString>

namespace Map
{
	void charArrayToQString(char charArray[200], QString& qString);

	void qStringToCharMatrix(QString qString, char charMatrix[8][8]);

	void charMatrixToQString(char charMatrix[8][8], QString& qString);
};
