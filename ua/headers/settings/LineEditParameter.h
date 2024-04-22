#pragma once

#include "AbstractParameter.h"
#include <QLineEdit>


class LineEditParameter : public AbstractParameter
{
private:
	QLineEdit* lineEdit;

public:
	LineEditParameter(std::string name);
	QString getValue();
	QWidget* getWidget();
};