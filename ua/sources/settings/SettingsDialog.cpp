#include "../../headers/settings/SettingsDialog.h"




SettingsDialog::SettingsDialog(QWidget* parent)
{
	cameraHandlerParametersWidgets = std::map<QString, AbstractParameter*> {
		{ "leftCameraIndex", new LineEditParameter("leftCameraIndex") },
		{ "rightCameraIndex", new LineEditParameter("rightCameraIndex") }
	};

	configureParametersWidgets = std::map<QString, AbstractParameter*> {
		{ "gaussianFilterDimension", new LineEditParameter("gaussianFilterDimension") },
		{ "binaryThreshold", new LineEditParameter("binaryThreshold") },
		{ "closingFilterDimension", new LineEditParameter("closingFilterDimension") },
		{ "houghRoStepSize", new LineEditParameter("houghRoStepSize") },
		{ "houghThetaStepSize", new LineEditParameter("houghThetaStepSize") },
		{ "houghWindowSize", new LineEditParameter("houghWindowSize") },
		{ "houghNumberOfLines", new LineEditParameter("houghNumberOfLines") },
		{ "showImages", new ComboBoxParameter("showImages", std::vector<QString>{"True", "False"}) },
		{ "concatImages", new ComboBoxParameter("concatImages", std::vector<QString>{"True", "False"}) }
	};

	borderParametersWidgets = std::map<QString, AbstractParameter*> {
		{ "borderTop", new LineEditParameter("borderTop") },
		{ "borderRight", new LineEditParameter("borderRight") },
		{ "borderBottom", new LineEditParameter("borderBottom") },
		{ "borderLeft", new LineEditParameter("borderLeft") }
	};

	cropAndLabelParametersWidgets = std::map<QString, AbstractParameter*>{
		{ "showImages", new ComboBoxParameter("showImages", std::vector<QString>{"True", "False"}) },
		{ "concatImages", new ComboBoxParameter("concatImages", std::vector<QString>{"True", "False"}) }
	};

	setWindowTitle("Settings");
	QLayout* centralLayout = new QVBoxLayout(this);

	centralLayout->addWidget(new QLabel("Camera handler parameters:"));
	centralLayout->addWidget(wrapParameters(cameraHandlerParametersWidgets));
	centralLayout->addWidget(new QLabel("Configure parameters:"));
	centralLayout->addWidget(wrapParameters(configureParametersWidgets));
	centralLayout->addWidget(new QLabel("Border parameters:"));
	centralLayout->addWidget(wrapParameters(borderParametersWidgets));
	centralLayout->addWidget(new QLabel("Crop and label parameters:"));
	centralLayout->addWidget(wrapParameters(cropAndLabelParametersWidgets));

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


QWidget* SettingsDialog::wrapParameters(std::map<QString, AbstractParameter*> parameterWidgets, int columns)
{
	QWidget* wrapperWidget = new QWidget();
	QGridLayout* wrapperLayout = new QGridLayout(wrapperWidget);

	int row = 0;
	int column = 0;

	auto it = parameterWidgets.begin();
	for (int i = 0; i < parameterWidgets.size(); i++)
	{
		QString name = it->first;
		AbstractParameter* widget = it->second;

		QLabel* label = new QLabel(widget->labelText);

		wrapperLayout->addWidget(label, row, column * 2);
		wrapperLayout->addWidget(widget->getWidget(), row, column * 2 + 1);

		column++;
		if (column >= columns)
		{
			row++;
			column = 0;
		}

		it++;
	}

	return wrapperWidget;
}


void SettingsDialog::saveButtonClicked()
{
	Parameters parameters;
	
	parameters.cameraHandlingParameters.cameraHandlerParameters.leftCameraIndex = cameraHandlerParametersWidgets["leftCameraIndex"]->getValue().toInt();
	parameters.cameraHandlingParameters.cameraHandlerParameters.rightCameraIndex = cameraHandlerParametersWidgets["rightCameraIndex"]->getValue().toInt();

	parameters.configurationParameters.configureParameters.gaussianFilterDimension = configureParametersWidgets["gaussianFilterDimension"]->getValue().toInt();
	parameters.configurationParameters.configureParameters.binaryThreshold = static_cast<uchar>(configureParametersWidgets["binaryThreshold"]->getValue().toInt());
	parameters.configurationParameters.configureParameters.closingFilterDimension = configureParametersWidgets["closingFilterDimension"]->getValue().toInt();
	parameters.configurationParameters.configureParameters.houghRoStepSize = configureParametersWidgets["houghRoStepSize"]->getValue().toInt();
	parameters.configurationParameters.configureParameters.houghThetaStepSize = configureParametersWidgets["houghThetaStepSize"]->getValue().toInt();
	parameters.configurationParameters.configureParameters.houghWindowSize = configureParametersWidgets["houghWindowSize"]->getValue().toInt();
	parameters.configurationParameters.configureParameters.houghNumberOfLines = configureParametersWidgets["houghNumberOfLines"]->getValue().toInt();
	parameters.configurationParameters.configureParameters.showImages = (configureParametersWidgets["showImages"]->getValue() == "True");
	parameters.configurationParameters.configureParameters.concatImages = (configureParametersWidgets["concatImages"]->getValue() == "True");

	parameters.configurationParameters.borderParameters.borderTop = borderParametersWidgets["borderTop"]->getValue().toInt();
	parameters.configurationParameters.borderParameters.borderRight = borderParametersWidgets["borderRight"]->getValue().toInt();
	parameters.configurationParameters.borderParameters.borderBottom = borderParametersWidgets["borderBottom"]->getValue().toInt();
	parameters.configurationParameters.borderParameters.borderLeft = borderParametersWidgets["borderLeft"]->getValue().toInt();

	parameters.configurationParameters.cropAndLabelParameters.showImages = (cropAndLabelParametersWidgets["showImages"]->getValue() == "True");
	parameters.configurationParameters.cropAndLabelParameters.concatImages = (cropAndLabelParametersWidgets["concatImages"]->getValue() == "True");

	emit setParametersSignal(parameters);
}


void SettingsDialog::refreshButtonClicked()
{
	emit getParametersSignal();
}


void SettingsDialog::cancelButtonClicked()
{
	done(0);
}


void SettingsDialog::setParametersReplySlot(bool succeeded, QString message)
{
	messageLabel->setText(message);
}


void SettingsDialog::getParametersReplySlot(Parameters parameters)
{
	cameraHandlerParametersWidgets["leftCameraIndex"]->setValue(QString::number(parameters.cameraHandlingParameters.cameraHandlerParameters.leftCameraIndex));
	cameraHandlerParametersWidgets["rightCameraIndex"]->setValue(QString::number(parameters.cameraHandlingParameters.cameraHandlerParameters.rightCameraIndex));

	configureParametersWidgets["gaussianFilterDimension"]->setValue(QString::number(parameters.configurationParameters.configureParameters.gaussianFilterDimension));
	configureParametersWidgets["binaryThreshold"]->setValue(QString::number(parameters.configurationParameters.configureParameters.binaryThreshold));  // number?
	configureParametersWidgets["closingFilterDimension"]->setValue(QString::number(parameters.configurationParameters.configureParameters.closingFilterDimension));
	configureParametersWidgets["houghRoStepSize"]->setValue(QString::number(parameters.configurationParameters.configureParameters.houghRoStepSize));
	configureParametersWidgets["houghThetaStepSize"]->setValue(QString::number(parameters.configurationParameters.configureParameters.houghThetaStepSize));
	configureParametersWidgets["houghWindowSize"]->setValue(QString::number(parameters.configurationParameters.configureParameters.houghWindowSize));
	configureParametersWidgets["houghNumberOfLines"]->setValue(QString::number(parameters.configurationParameters.configureParameters.houghNumberOfLines));
	configureParametersWidgets["showImages"]->setValue((parameters.configurationParameters.configureParameters.showImages) ? "True" : "False");
	configureParametersWidgets["concatImages"]->setValue((parameters.configurationParameters.configureParameters.concatImages) ? "True" : "False");

	borderParametersWidgets["borderTop"]->setValue(QString::number(parameters.configurationParameters.borderParameters.borderTop));
	borderParametersWidgets["borderRight"]->setValue(QString::number(parameters.configurationParameters.borderParameters.borderRight));
	borderParametersWidgets["borderBottom"]->setValue(QString::number(parameters.configurationParameters.borderParameters.borderBottom));
	borderParametersWidgets["borderLeft"]->setValue(QString::number(parameters.configurationParameters.borderParameters.borderLeft));

	cropAndLabelParametersWidgets["showImages"]->setValue((parameters.configurationParameters.cropAndLabelParameters.showImages) ? "True" : "False");
	cropAndLabelParametersWidgets["concatImages"]->setValue((parameters.configurationParameters.cropAndLabelParameters.concatImages) ? "True" : "False");
}

