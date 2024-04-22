#include "../../headers/settings/LineEditParameter.h"


LineEditParameter::LineEditParameter(std::string name)
{
	this->name = name;

	lineEdit = new QLineEdit();
}


QWidget* LineEditParameter::getWidget()
{
	return lineEdit;
}


QString LineEditParameter::getValue()
{
	return lineEdit->text();
}