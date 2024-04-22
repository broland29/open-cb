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


std::string LineEditParameter::getValue()
{
	return "123";
}