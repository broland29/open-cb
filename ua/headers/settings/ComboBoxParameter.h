#pragma once

#include <QComboBox>

#include "AbstractParameter.h"


class ComboBoxParameter : public AbstractParameter
{
public:

private:
	QComboBox* comboBox;

public:
	ComboBoxParameter(QString labelText, std::vector<QString> options);

	QString getValue();
	
	void setValue(QString value);
	
	QWidget* getWidget();

private:
};