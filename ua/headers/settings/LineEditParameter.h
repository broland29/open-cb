#pragma once

#include "AbstractParameter.h"
#include <QLineEdit>


class LineEditParameter : public AbstractParameter
{
private:
	QLineEdit* lineEdit;

public:
	LineEditParameter(QString labelText);
	QString getValue();
	void setValue(QString value);
	QWidget* getWidget();
};