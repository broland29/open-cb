#pragma once
#include "AbstractParameter.h"
#include <QComboBox>


class ComboBoxParameter : public AbstractParameter
{
private:
	QComboBox* comboBox;

public:
	ComboBoxParameter(QString labelText, std::vector<QString> options);
	QString getValue();
	void setValue(QString value);
	QWidget* getWidget();
};