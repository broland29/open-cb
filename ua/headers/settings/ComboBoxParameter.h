#pragma once
#include "AbstractParameter.h"
#include <QComboBox>


class ComboBoxParameter : public AbstractParameter
{
private:
	QComboBox* comboBox;

public:
	ComboBoxParameter(std::string name, std::vector<std::string> options);
	QString getValue();
	QWidget* getWidget();
};