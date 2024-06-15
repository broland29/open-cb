#include "../../headers/settings/SettingsDialog.h"




SettingsDialog::SettingsDialog(QWidget* parent)
{
	this->setStyleSheet("background-color:" + Style::LIGHT);

	cameraHandlerParametersWidgets = std::map<QString, AbstractParameter*> {
		{ "leftCameraIndex", new LineEditParameter("leftCameraIndex") },
		{ "rightCameraIndex", new LineEditParameter("rightCameraIndex") }
	};

	KNNParametersWidgets = std::map<QString, AbstractParameter*> {
		{ "k", new LineEditParameter("k") },
		{ "numberOfBins", new LineEditParameter("numberOfBins") },
		{ "uniteFrees", new ComboBoxParameter("uniteFrees", std::vector<QString>{"True", "False"}) }
	};

	SVMParametersWidgets = std::map<QString, AbstractParameter*>{
		{ "uniteFrees", new ComboBoxParameter("uniteFrees", std::vector<QString>{"True", "False"}) }
	};

	CNNParametersWidgets = std::map<QString, AbstractParameter*>{
		{ "epochs", new LineEditParameter("epochs") },
		{ "applyAugmentation", new ComboBoxParameter("applyAugmentation", std::vector<QString>{"True", "False"}) }
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

	cropAndLabelParametersWidgets = std::map<QString, AbstractParameter*> {
		{ "showImages", new ComboBoxParameter("showImages", std::vector<QString>{"True", "False"}) },
		{ "concatImages", new ComboBoxParameter("concatImages", std::vector<QString>{"True", "False"}) }
	};

	fileHandlerParametersWidgets = std::map<QString, AbstractParameter*> {
		{ "trainSplit", new LineEditParameter("trainSplit") },
		{ "validationSplit", new LineEditParameter("validationSplit") },
		{ "testSplit", new LineEditParameter("testSplit") }
	};

	setWindowTitle("Settings");
	QLayout* centralLayout = new QVBoxLayout(this);

	centralLayout->addWidget(new QLabel("Camera handler parameters:"));
	centralLayout->addWidget(wrapParameters(cameraHandlerParametersWidgets));
	centralLayout->addWidget(new QLabel("KNN parameters:"));
	centralLayout->addWidget(wrapParameters(KNNParametersWidgets));
	centralLayout->addWidget(new QLabel("SVM parameters:"));
	centralLayout->addWidget(wrapParameters(SVMParametersWidgets));
	centralLayout->addWidget(new QLabel("CNN parameters:"));
	centralLayout->addWidget(wrapParameters(CNNParametersWidgets));
	centralLayout->addWidget(new QLabel("Configure parameters:"));
	centralLayout->addWidget(wrapParameters(configureParametersWidgets));
	centralLayout->addWidget(new QLabel("Border parameters:"));
	centralLayout->addWidget(wrapParameters(borderParametersWidgets));
	centralLayout->addWidget(new QLabel("Crop and label parameters:"));
	centralLayout->addWidget(wrapParameters(cropAndLabelParametersWidgets));
	centralLayout->addWidget(new QLabel("File handler parameters:"));
	centralLayout->addWidget(wrapParameters(fileHandlerParametersWidgets));

	QWidget* messageWidget = new QWidget();
	QHBoxLayout* messageLayout = new QHBoxLayout(messageWidget);
	messageWidget->setStyleSheet("background-color:" + Style::LIGHTEST);
	messageLabel = new QLabel("Welcome to the Settings Panel!");
	messageLayout->addWidget(messageLabel);
	centralLayout->addWidget(messageWidget);

	QWidget* buttonWidget = new QWidget();
	QHBoxLayout* buttonLayout = new QHBoxLayout(buttonWidget);
	restoreDefaultsButton = new QPushButton("Restore Defaults");
	saveButton = new QPushButton("Save");
	refreshButton = new QPushButton("Refresh");
	cancelButton = new QPushButton("Cancel");
	restoreDefaultsButton->setStyleSheet("background-color:" + Style::DARK);
	saveButton->setStyleSheet("background-color:" + Style::DARK);
	refreshButton->setStyleSheet("background-color:" + Style::DARK);
	cancelButton->setStyleSheet("background-color:" + Style::DARK);
	buttonLayout->addWidget(restoreDefaultsButton);
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

	wrapperWidget->setStyleSheet("background-color:" + Style::LIGHT);

	int row = 0;
	int column = 0;

	auto it = parameterWidgets.begin();
	for (int i = 0; i < parameterWidgets.size(); i++)
	{
		QString name = it->first;
		AbstractParameter* widget = it->second;

		QLabel* label = new QLabel("   " + widget->labelText + ":");

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

	// fill remaining space with dummy labels
	while (column % 3 != 0)
	{
		wrapperLayout->addWidget(new QLabel(""), row, column * 2);
		wrapperLayout->addWidget(new QLabel(""), row, column * 2 + 1);
		column++;
	}

	return wrapperWidget;
}


void SettingsDialog::saveButtonClicked()
{
	Parameters parameters;
	
	parameters.cameraHandlingParameters.cameraHandlerParameters.leftCameraIndex = cameraHandlerParametersWidgets["leftCameraIndex"]->getValue().toInt();
	parameters.cameraHandlingParameters.cameraHandlerParameters.rightCameraIndex = cameraHandlerParametersWidgets["rightCameraIndex"]->getValue().toInt();

	parameters.classificationParameters.kNearestNeighborsParameters.k = KNNParametersWidgets["k"]->getValue().toInt();
	parameters.classificationParameters.kNearestNeighborsParameters.numberOfBins = KNNParametersWidgets["numberOfBins"]->getValue().toInt();
	parameters.classificationParameters.kNearestNeighborsParameters.uniteFrees = (KNNParametersWidgets["uniteFrees"]->getValue() == "True");

	parameters.classificationParameters.supportVectorMachineParameters.uniteFrees = (SVMParametersWidgets["uniteFrees"]->getValue() == "True");

	parameters.classificationParameters.convolutionalNeuralNetworkParameters.epochs = CNNParametersWidgets["epochs"]->getValue().toInt();
	parameters.classificationParameters.convolutionalNeuralNetworkParameters.applyAugmentation = (CNNParametersWidgets["applyAugmentation"]->getValue() == "True");

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

	parameters.fileHandlingParameters.fileHandlerParameters.trainSplit = fileHandlerParametersWidgets["trainSplit"]->getValue().toDouble();
	parameters.fileHandlingParameters.fileHandlerParameters.validationSplit = fileHandlerParametersWidgets["validationSplit"]->getValue().toDouble();
	parameters.fileHandlingParameters.fileHandlerParameters.testSplit = fileHandlerParametersWidgets["testSplit"]->getValue().toDouble();

	emit setParametersSignal(parameters);
}

void SettingsDialog::getParametersReplySlot(Parameters parameters)
{
	cameraHandlerParametersWidgets["leftCameraIndex"]->setValue(QString::number(parameters.cameraHandlingParameters.cameraHandlerParameters.leftCameraIndex));
	cameraHandlerParametersWidgets["rightCameraIndex"]->setValue(QString::number(parameters.cameraHandlingParameters.cameraHandlerParameters.rightCameraIndex));

	KNNParametersWidgets["k"]->setValue(QString::number(parameters.classificationParameters.kNearestNeighborsParameters.k));
	KNNParametersWidgets["numberOfBins"]->setValue(QString::number(parameters.classificationParameters.kNearestNeighborsParameters.numberOfBins));
	KNNParametersWidgets["uniteFrees"]->setValue((parameters.classificationParameters.kNearestNeighborsParameters.uniteFrees) ? "True" : "False");

	SVMParametersWidgets["uniteFrees"]->setValue((parameters.classificationParameters.supportVectorMachineParameters.uniteFrees) ? "True" : "False");

	CNNParametersWidgets["epochs"]->setValue(QString::number(parameters.classificationParameters.convolutionalNeuralNetworkParameters.epochs));
	CNNParametersWidgets["applyAugmentation"]->setValue((parameters.classificationParameters.convolutionalNeuralNetworkParameters.applyAugmentation) ? "True" : "False");

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

	fileHandlerParametersWidgets["trainSplit"]->setValue(QString::number(parameters.fileHandlingParameters.fileHandlerParameters.trainSplit));
	fileHandlerParametersWidgets["validationSplit"]->setValue(QString::number(parameters.fileHandlingParameters.fileHandlerParameters.validationSplit));
	fileHandlerParametersWidgets["testSplit"]->setValue(QString::number(parameters.fileHandlingParameters.fileHandlerParameters.testSplit));
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
