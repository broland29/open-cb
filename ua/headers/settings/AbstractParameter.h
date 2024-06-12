#pragma once
#include <string>
#include <QWidget>
#include <QString>
#include "../Style.h"
#include <spdlog/spdlog.h>


class AbstractParameter
{
public:
	QString labelText;

public:
	virtual QString getValue() = 0;
	virtual void setValue(QString value) = 0;
	virtual QWidget* getWidget() = 0;
};