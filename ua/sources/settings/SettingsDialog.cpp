#include "../../headers/settings/SettingsDialog.h"


SettingsDialog::SettingsDialog(QWidget* parent)
{
	// define all widgets for parameters. key is internal/JSON name
	parameterWidgets = std::map<QString, AbstractParameter*> {
		{ "showImages", new ComboBoxParameter("Show images", std::vector<QString>{"True", "False"}) },
		{ "concatImages", new ComboBoxParameter("Concat images", std::vector<QString>{"True", "False"}) },
		{ "leftCameraIndex", new LineEditParameter("Left camera index") },
		{ "rightCameraIndex", new LineEditParameter("Right camera index") },
		{ "borderTop", new LineEditParameter("Border top") },
		{ "borderRight", new LineEditParameter("Border right") },
		{ "borderBottom", new LineEditParameter("Border bottom") },
		{ "borderLeft", new LineEditParameter("Border left") },
		{ "imageWidth", new LineEditParameter("Image width") },
		{ "imageHeight", new LineEditParameter("Image height") },
		{ "binaryThreshold", new LineEditParameter("Binary Threshold") },
		{ "closingSize", new LineEditParameter("Closing size") },
		{ "houghRo", new LineEditParameter("Hough ro") },
		{ "houghTheta", new LineEditParameter("Hough theta") },
		{ "houghWindowSize", new LineEditParameter("Hough window size") },
		{ "houghNoOfLines", new LineEditParameter("Hough no of lines") },
		{ "KNN", new LineEditParameter("KNN") },
		{ "SVM", new LineEditParameter("SVM") },
		{ "CNN", new LineEditParameter("CNN") },
	};

	// put widgets in separate containers to use utility function wrapParameters
	std::vector<AbstractParameter*> visualizationParameters = {
		parameterWidgets["showImages"],
		parameterWidgets["concatImages"],
	};
	std::vector<AbstractParameter*> cameraParameters = {
		parameterWidgets["leftCameraIndex"],
		parameterWidgets["rightCameraIndex"],
	};
	std::vector<AbstractParameter*> configurationParameters = {
		parameterWidgets["borderTop"],
		parameterWidgets["borderRight"],
		parameterWidgets["borderBottom"],
		parameterWidgets["borderLeft"],
		parameterWidgets["imageWidth"],
		parameterWidgets["imageHeight"],
		parameterWidgets["binaryThreshold"],
		parameterWidgets["closingSize"],
		parameterWidgets["houghRo"],
		parameterWidgets["houghTheta"],
		parameterWidgets["houghWindowSize"],
		parameterWidgets["houghNoOfLines"],
	};
	std::vector<AbstractParameter*> classificationParameters = {
		parameterWidgets["KNN"],
		parameterWidgets["SVM"],
		parameterWidgets["CNN"],
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

	messageLabel = new QLabel("Message will appear here");
	centralLayout->addWidget(messageLabel);

	QWidget* buttonWidget = new QWidget();
	QHBoxLayout* buttonLayout = new QHBoxLayout(buttonWidget);
	saveButton = new QPushButton("Save");
	refreshButton = new QPushButton("Refresh");
	cancelButton = new QPushButton("Cancel");
	buttonLayout->addWidget(saveButton);
	buttonLayout->addWidget(refreshButton);
	buttonLayout->addWidget(cancelButton);
	centralLayout->addWidget(buttonWidget);

	QObject::connect(saveButton, &QPushButton::clicked, this, &SettingsDialog::saveButtonClicked);
	QObject::connect(refreshButton, &QPushButton::clicked, this, &SettingsDialog::refreshButtonClicked);
	QObject::connect(cancelButton, &QPushButton::clicked, this, &SettingsDialog::cancelButtonClicked);

	// MainWindow shall emit getParametersSignal to fill fields after constructing and tieing slots and signals
}


void loadParameters()
{

}


QWidget* SettingsDialog::wrapParameters(std::vector<AbstractParameter*> parameters, int columns)
{
	QWidget* widget = new QWidget();
	QGridLayout* layout = new QGridLayout(widget);

	int row = 0;
	int column = 0;

	for (int i = 0; i < parameters.size(); i++)
	{
		QLabel* label = new QLabel(parameters[i]->labelText);

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


void SettingsDialog::saveButtonClicked()
{
	QVector<QString> names;
	QVector<QString> values;

	names.push_back("leftCameraIndex");
	values.push_back(parameterWidgets["leftCameraIndex"]->getValue());
	names.push_back("rightCameraIndex");
	values.push_back(parameterWidgets["rightCameraIndex"]->getValue());
	names.push_back("showImages");
	values.push_back(parameterWidgets["showImages"]->getValue());
	names.push_back("concatImages");
	values.push_back(parameterWidgets["concatImages"]->getValue());

	if (names.size() != values.size())
	{
		SPDLOG_ERROR("Size mismatch");
		return;
	}

	SPDLOG_TRACE("Sending parameters:");
	for (int i = 0; i < names.size(); i++)
	{
		SPDLOG_TRACE("{}:{}", names[i].toStdString(), values[i].toStdString());
	}

	emit setParametersSignal(names, values);
}


void SettingsDialog::refreshButtonClicked()
{
	QVector<QString> names;
	names.push_back("leftCameraIndex");
	names.push_back("rightCameraIndex");
	names.push_back("showImages");
	names.push_back("concatImages");
	emit getParametersSignal(names);
}


void SettingsDialog::cancelButtonClicked()
{
	done(0);
}


void SettingsDialog::setParametersReplySlot(bool succeeded, QString message)
{
	messageLabel->setText(message);
}





void SettingsDialog::getParametersReplySlot(QVector<QString> names, QVector<QString> values)
{
	if (names.length() != values.length())
	{
		SPDLOG_ERROR("Size mismatch");
		return;
	}

	for (int i = 0; i < names.size(); i++)
	{
		if (parameterWidgets.find(names[i]) == parameterWidgets.end())
		{
			SPDLOG_ERROR("Widget with name {} not found", names[i].toStdString());
			continue;
		}
		parameterWidgets[names[i]]->setValue(values[i]);
	}
}

