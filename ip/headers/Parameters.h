#pragma once

#include <QString>
#include <QVector>
#include <variant>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <map>
#include <iostream>


typedef std::variant<bool, int, double, std::string> Variant;

class Parameters
{
public:
	std::map<QString, Variant> parameters = {
		{ "leftCameraIndex",	1		},
		{ "rightCameraIndex",	2		},
		{ "showImages",			true	},
		{ "concatImages",		true	},
		{ "borderTop",			53		},
		{ "borderRight",		50		},
		{ "borderBottom",		55		},
		{ "borderLeft",			70		},
	};

private:
public:
	static void variantToQString(Variant variant, QString& qString);
	static void variantToStdString(Variant variant, std::string& stdString);

	void getValue(QString name, QString& value);
	void getValue(QString name, std::string& value);
	void setValue(QString name, QString value);
	void setValue(QString name, Variant value);
	void getValues(QVector<QString> names, QVector<QString>& values);
	void setValues(QVector<QString> names, QVector<QString> values);

private:
};
std::ostream& operator << (std::ostream& os, const std::map<QString, Variant>& m);  // https://stackoverflow.com/a/9230853/17694100


template <> struct fmt::formatter<std::map<QString, Variant>> : ostream_formatter {};

//template <> struct fmt::formatter<Castle> : ostream_formatter {};
//template <> struct fmt::formatter<Mat_<uchar>> : ostream_formatter {};


