#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QLayout>
#include "AbstractParameter.h"
#include "ComboBoxParameter.h"
#include "LineEditParameter.h"


// https://doc.qt.io/qt-6/application-windows.html
// https://doc.qt.io/qt-6/dialogs.html
// https://doc.qt.io/qt-6/qdialog.html


class SettingsDialog : public QDialog
{
public:

private:
	const int columns = 3;
	
	std::vector<AbstractParameter*> visualizationParameters;

public:
	SettingsDialog(QWidget* patent);

private:
	QWidget* wrapParameters(std::vector<AbstractParameter*> parameters);

};