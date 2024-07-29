#include "../../headers/settings/LineEditParameter.h"


LineEditParameter::LineEditParameter(QString labelText)
{
	this->labelText = labelText;

	lineEdit = new QLineEdit();
	lineEdit->setMaximumWidth(150);
	lineEdit->setStyleSheet("background-color:" + Style::LIGHTEST);
}


QWidget* LineEditParameter::getWidget()
{
	return lineEdit;
}


QString LineEditParameter::getValue()
{
	return lineEdit->text();
}


void LineEditParameter::setValue(QString value)
{
	lineEdit->setText(value);
}
