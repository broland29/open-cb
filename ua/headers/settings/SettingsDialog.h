#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QLayout>
#include <QPushButton>
#include <QVector>
#include <QString>
#include "AbstractParameter.h"
#include "ComboBoxParameter.h"
#include "LineEditParameter.h"
#include <iterator>

#include <spdlog/spdlog.h>


// https://doc.qt.io/qt-6/application-windows.html
// https://doc.qt.io/qt-6/dialogs.html
// https://doc.qt.io/qt-6/qdialog.html


class SettingsDialog : public QDialog
{
	Q_OBJECT

public:

private:	
	std::map<QString, AbstractParameter*> parameterWidgets;

	QLabel* messageLabel;

	QPushButton* saveButton;
	QPushButton* refreshButton;
	QPushButton* cancelButton;

public:
	SettingsDialog(QWidget* patent);

private:
	QWidget* wrapParameters(std::vector<AbstractParameter*> parameters, int columns = 3);

public slots:
	void saveButtonClicked();
	void refreshButtonClicked();
	void cancelButtonClicked();

	void setParametersReplySlot(bool succeeded, QString message);
	void getParametersReplySlot(QVector<QString> names, QVector<QString> values);
signals:
	void setParametersSignal(QVector<QString> names, QVector<QString> values);
	void getParametersSignal(QVector<QString> names);
};