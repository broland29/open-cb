#include "../../headers/settings/SettingsDialog.h"


SettingsDialog::SettingsDialog(QWidget* parent)
{
	// define parameters
	visualizationParameters = std::vector<AbstractParameter*>{
		new ComboBoxParameter("Show images", std::vector<std::string>{"True", "False"}),
		new ComboBoxParameter("Concat images", std::vector<std::string>{"True", "False"}),
	};
	cameraParameters = std::vector<AbstractParameter*>{
		new LineEditParameter("Left camera index"),
		new LineEditParameter("Right camera index")
	};
	configurationParameters = std::vector<AbstractParameter*>{
		new LineEditParameter("Border top"),
		new LineEditParameter("Border right"),
		new LineEditParameter("Border bottom"),
		new LineEditParameter("Border left"),
		new LineEditParameter("Image width"),
		new LineEditParameter("Image height"),
		new LineEditParameter("Binary Threshold"),
		new LineEditParameter("Closing size"),
		new LineEditParameter("Hough ro"),
		new LineEditParameter("Hough theta"),
		new LineEditParameter("Hough window size"),
		new LineEditParameter("Hough no of lines")
	};
	classificationParameters = std::vector<AbstractParameter*>{
		new LineEditParameter("KNN"),
		new LineEditParameter("SVM"),
		new LineEditParameter("CNN")
	};
	pathParameters = std::vector<AbstractParameter*>{
		new LineEditParameter("Images path")
	};

	setWindowTitle("Settings");
	QLayout* centralLayout = new QVBoxLayout(this);

	centralLayout->addWidget(new QLabel("Visualization parameters:"));
	centralLayout->addWidget(wrapParameters(visualizationParameters));
	centralLayout->addWidget(new QLabel("Camera parameters:"));
	centralLayout->addWidget(wrapParameters(cameraParameters));
	centralLayout->addWidget(new QLabel("Configuration parameters:"));
	centralLayout->addWidget(wrapParameters(configurationParameters));
	centralLayout->addWidget(new QLabel("Classification parameters:"));
	centralLayout->addWidget(wrapParameters(classificationParameters));
	centralLayout->addWidget(new QLabel("Path parameters:"));
	centralLayout->addWidget(wrapParameters(pathParameters));

	messageLabel = new QLabel("Message will appear here");
	centralLayout->addWidget(messageLabel);

	QWidget* buttonWidget = new QWidget();
	QHBoxLayout* buttonLayout = new QHBoxLayout(buttonWidget);
	saveButton = new QPushButton("Save");
	cancelButton = new QPushButton("Cancel");
	buttonLayout->addWidget(saveButton);
	buttonLayout->addWidget(cancelButton);
	centralLayout->addWidget(buttonWidget);

	QObject::connect(saveButton, &QPushButton::clicked, this, &SettingsDialog::saveButtonClicked);
	QObject::connect(cancelButton, &QPushButton::clicked, this, &SettingsDialog::cancelButtonClicked);
}


void loadParameters()
{

}


QWidget* SettingsDialog::wrapParameters(std::vector<AbstractParameter*> parameters)
{
	QWidget* widget = new QWidget();
	QGridLayout* layout = new QGridLayout(widget);

	int row = 0;
	int column = 0;

	for (int i = 0; i < parameters.size(); i++)
	{
		QLabel* label = new QLabel(parameters[i]->name.c_str());

		layout->addWidget(label, row, column * 2);
		layout->addWidget(parameters[i]->getWidget(), row, column * 2 + 1);

		column++;
		if (column >= columns)
		{
			row++;
			column = 0;
		}
	}

	return widget;
}


void logParameters(std::vector<AbstractParameter*> parameters)
{
	for (AbstractParameter* parameter : parameters)
	{
		SPDLOG_TRACE("{}:{}", parameter->name, parameter->getValue().toStdString());
	}
	SPDLOG_TRACE("");
}

void SettingsDialog::saveButtonClicked()
{
	SPDLOG_TRACE("Parameters saved are:");
	logParameters(visualizationParameters);
	logParameters(cameraParameters);
	logParameters(configurationParameters);
	logParameters(classificationParameters);
	logParameters(pathParameters);

	QVector<QString> changedSignalNames;
	QVector<QString> changedSignalValues;
	changedSignalNames.push_back("leftCameraIndex");
	changedSignalValues.push_back(cameraParameters[0]->getValue());
	changedSignalNames.push_back("rightCameraIndex");
	changedSignalValues.push_back(cameraParameters[1]->getValue());

	emit parametersChangedSignal(changedSignalNames, changedSignalValues);
}

void SettingsDialog::parametersChangedReplySlot(bool succeeded, QString message)
{
	messageLabel->setText(message);
}


void SettingsDialog::cancelButtonClicked()
{

}