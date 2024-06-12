#include "../../headers/settings/ComboBoxParameter.h"


ComboBoxParameter::ComboBoxParameter(QString labelText, std::vector<QString> options)
{
	this->labelText = labelText;

	comboBox = new QComboBox();
	for (QString option : options)
	{
		comboBox->addItem(option);
	}
	comboBox->setMaximumWidth(150);
	comboBox->setStyleSheet("background-color:" + Style::DARK);
}


QWidget* ComboBoxParameter::getWidget()
{
	return comboBox;
}


QString ComboBoxParameter::getValue()
{
	return comboBox->currentText();
}


void ComboBoxParameter::setValue(QString value)
{
	if (comboBox->findText(value) == -1)
	{
		SPDLOG_ERROR("Value {} is not a valid option for {}", value.toStdString(), labelText.toStdString());
		return;
	}
	comboBox->setCurrentText(value);
}