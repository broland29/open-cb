#pragma once

#include <string>
#include <QWidget>
#include <QString>

#include <spdlog/spdlog.h>

#include "../Style.h"



class AbstractParameter
{
public:
	QString labelText;

private:

public:
	virtual QString getValue() = 0;

	virtual void setValue(QString value) = 0;

	virtual QWidget* getWidget() = 0;

private:
};