#include "../../headers/settings/ComboBoxParameter.h"


ComboBoxParameter::ComboBoxParameter(std::string name, std::vector<std::string> options)
{
	this->name = name;

	comboBox = new QComboBox();
	for (std::string option : options)
	{
		comboBox->addItem(option.c_str());
	}
}


QWidget* ComboBoxParameter::getWidget()
{
	return comboBox;
}


QString ComboBoxParameter::getValue()
{
	return comboBox->currentText();
}