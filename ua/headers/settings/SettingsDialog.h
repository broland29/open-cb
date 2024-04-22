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

#include <spdlog/spdlog.h>


// https://doc.qt.io/qt-6/application-windows.html
// https://doc.qt.io/qt-6/dialogs.html
// https://doc.qt.io/qt-6/qdialog.html


class SettingsDialog : public QDialog
{
	Q_OBJECT

public:

private:
	const int columns = 3;
	
	std::vector<AbstractParameter*> visualizationParameters;
	std::vector<AbstractParameter*> cameraParameters;
	std::vector<AbstractParameter*> configurationParameters;
	std::vector<AbstractParameter*> classificationParameters;
	std::vector<AbstractParameter*> pathParameters;

	QLabel* messageLabel;

	QPushButton* saveButton;
	QPushButton* cancelButton;

public:
	SettingsDialog(QWidget* patent);

private:
	QWidget* wrapParameters(std::vector<AbstractParameter*> parameters);

public slots:
	void saveButtonClicked();
	void cancelButtonClicked();

	void parametersChangedReplySlot(bool succeeded, QString message);

signals:
	void parametersChangedSignal(QVector<QString> changedSignalNames, QVector<QString> changedSignalValues);
};