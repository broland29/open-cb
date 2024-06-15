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
#include "../com/headers/Parameters.h"
#include "../Style.h"

#include <spdlog/spdlog.h>


// https://doc.qt.io/qt-6/application-windows.html
// https://doc.qt.io/qt-6/dialogs.html
// https://doc.qt.io/qt-6/qdialog.html


class SettingsDialog : public QDialog
{
	Q_OBJECT

public:

private:
	std::map<QString, AbstractParameter*> cameraHandlerParametersWidgets;
	std::map<QString, AbstractParameter*> KNNParametersWidgets;
	std::map<QString, AbstractParameter*> SVMParametersWidgets;
	std::map<QString, AbstractParameter*> CNNParametersWidgets;
	std::map<QString, AbstractParameter*> configureParametersWidgets;
	std::map<QString, AbstractParameter*> borderParametersWidgets;
	std::map<QString, AbstractParameter*> cropAndLabelParametersWidgets;
	std::map<QString, AbstractParameter*> fileHandlerParametersWidgets;

	QLabel* messageLabel;

	QPushButton* restoreDefaultsButton;
	QPushButton* saveButton;
	QPushButton* refreshButton;
	QPushButton* cancelButton;

public:
	SettingsDialog(QWidget* patent);

private:
	QWidget* wrapParameters(std::map<QString, AbstractParameter*> parameterWidgets, int columns = 3);

public slots:
	void saveButtonClicked();
	void refreshButtonClicked();
	void cancelButtonClicked();

	void setParametersReplySlot(bool succeeded, QString message);
	void getParametersReplySlot(Parameters parameters);

signals:
	void setParametersSignal(Parameters parameters);
	void getParametersSignal();
};