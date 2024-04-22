#include "../../headers/settings/SettingsDialog.h"


SettingsDialog::SettingsDialog(QWidget* parent)
{
	setWindowTitle("Settings");

	QLayout* centralLayout = new QVBoxLayout(this);

	visualizationParameters = std::vector<AbstractParameter*>{
		new LineEditParameter("Par1"),
		new LineEditParameter("Par2"),
		new LineEditParameter("Par3"),
		new LineEditParameter("Par4"),
		new ComboBoxParameter("Par5", std::vector<std::string>{"True", "False"}),
		new ComboBoxParameter("Par6", std::vector<std::string>{"True", "False"}),
		new ComboBoxParameter("Par7", std::vector<std::string>{"One", "Two", "Three"}),
		new LineEditParameter("Par8"),
		new LineEditParameter("Par9"),
		new ComboBoxParameter("Par10", std::vector<std::string>{"True", "False"}),
	};

	centralLayout->addWidget(wrapParameters(visualizationParameters));

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