#pragma once
#include <string>
#include <QWidget>


class AbstractParameter
{
public:
	std::string name;

public:
	virtual std::string getValue() = 0;
	virtual QWidget* getWidget() = 0;
};