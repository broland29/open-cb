#include "../headers/Parameters.h"


void Parameters::variantToQString(Variant variant, QString& qString)
{
	if (std::holds_alternative<bool>(variant))
	{
		qString = QString::number(std::get<bool>(variant));  // should work
	}
	else if (std::holds_alternative<int>(variant))
	{
		qString = QString::number(std::get<int>(variant));
	}
	else if (std::holds_alternative<double>(variant))
	{
		qString = QString::number(std::get<double>(variant));
	}
	else if (std::holds_alternative<std::string>(variant))
	{
		qString = QString::fromStdString(std::get<std::string>(variant));
	}
	else
	{
		SPDLOG_ERROR("Error converting Variant to QString");
	}
}

void Parameters::variantToStdString(Variant variant, std::string& stdString)
{
	if (std::holds_alternative<bool>(variant))
	{
		stdString = std::to_string(std::get<bool>(variant));
	}
	else if (std::holds_alternative<int>(variant))
	{
		stdString = std::to_string(std::get<int>(variant));
	}
	else if (std::holds_alternative<double>(variant))
	{
		stdString = std::to_string(std::get<double>(variant));
	}
	else if (std::holds_alternative<std::string>(variant))
	{
		stdString = std::get<std::string>(variant);
	}
	else
	{
		SPDLOG_ERROR("Error converting Variant to std::string");
	}
}


std::ostream& operator << (std::ostream& os, const std::map<QString, Variant>& m)
{
	for (const auto& p : m)
	{
		std::string name = p.first.toStdString();
		std::string value;
		Parameters::variantToStdString(p.second, value);
		os << std::endl << name << " " << value;
	}
	return os;
}


void Parameters::getValue(QString name, QString& value)
{
	if (parameters.find(name) == parameters.end())
	{
		SPDLOG_ERROR("Parameter {} not registered", name.toStdString());
		return;
	}
	variantToQString(parameters[name], value);
}


void Parameters::getValue(QString name, std::string& value)
{
	if (parameters.find(name) == parameters.end())
	{
		SPDLOG_ERROR("Parameter {} not registered", name.toStdString());
		return;
	}
	variantToStdString(parameters[name], value);
}


void Parameters::setValue(QString name, QString value)
{
	if (parameters.find(name) == parameters.end())
	{
		SPDLOG_ERROR("Parameter {} not registered", name.toStdString());
		return;
	}

	std::variant value_ = parameters[name];

	bool success = true;  // since toStdString has no arg, assume success
	if (std::holds_alternative<bool>(value_))
	{
		parameters[name] = value.toInt(&success);  // should work
	}
	else if (std::holds_alternative<int>(value_))
	{
		parameters[name] = value.toInt(&success);
	}
	else if (std::holds_alternative<double>(value_))
	{
		parameters[name] = value.toDouble(&success);
	}
	else if (std::holds_alternative<std::string>(value_))
	{
		parameters[name] = value.toStdString();
	}
	else
	{
		SPDLOG_ERROR("Error setting parameter {}: variant type not handled", name.toStdString());
		return;
	}

	if (!success)
	{
		SPDLOG_ERROR("Error converting parameter {}: conversion failed, bad input", name.toStdString());
		return;
	}
}


void Parameters::setValue(QString name, Variant value)
{
	parameters[name] = value;
}


void Parameters::getValues(QVector<QString> names, QVector<QString>& values)
{
	if (names.size() != values.size())
	{
		SPDLOG_ERROR("Got vectors of different length");
		return;
	}

	for (int i = 0; i < names.size(); i++)
	{
		getValue(names[i], values[i]);
	}
}


void Parameters::setValues(QVector<QString> names, QVector<QString> values)
{
	SPDLOG_INFO("Parameters before update: {}", parameters);
	if (names.size() != values.size())
	{
		SPDLOG_ERROR("Got vectors of different length");
		return;
	}

	for (int i = 0; i < names.size(); i++)
	{
		setValue(names[i], values[i]);
	}

	SPDLOG_INFO("Parameters after update: {}", parameters);
}
