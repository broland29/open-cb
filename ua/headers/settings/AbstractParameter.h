#pragma once
#include <string>
#include <QWidget>
#include <QString>


class AbstractParameter
{
public:
	std::string name;

public:
	virtual QString getValue() = 0;
	virtual QWidget* getWidget() = 0;
};