#pragma once
#include "AbstractParameter.h"
#include <QComboBox>


class ComboBoxParameter : public AbstractParameter
{
private:
	QComboBox* comboBox;

public:
	ComboBoxParameter(std::string name, std::vector<std::string> options);
	std::string getValue();
	QWidget* getWidget();
};