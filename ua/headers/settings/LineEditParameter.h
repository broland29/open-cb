#pragma once

#include <QLineEdit>

#include "AbstractParameter.h"


class LineEditParameter : public AbstractParameter
{
public:

private:
	QLineEdit* lineEdit;

public:
	LineEditParameter(QString labelText);

	QString getValue();

	void setValue(QString value);

	QWidget* getWidget();

private:
};