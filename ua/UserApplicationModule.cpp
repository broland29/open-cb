// https://www.youtube.com/watch?v=Wi9nQTDFF4U&t=5208s

#include "UserApplicationModule.h"
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QMenu>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QTime>
#include <QTimer>
#include <QListWidget>

UserApplicationModule::UserApplicationModule(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("OpenCB");
    setFixedSize(800, 700);

    QWidget *centralWidget = new QWidget;
    QVBoxLayout* centralLayout = new QVBoxLayout();

    // -- top -- //
    QWidget* topWidget = new QWidget;
    QHBoxLayout* topLayout = new QHBoxLayout(topWidget);
    QLabel* topLabel = new QLabel("OpenCB User Application Beta");
    topLayout->addWidget(topLabel, 0, Qt::AlignCenter);
    topWidget->setStyleSheet("background-color:yellow");
    topWidget->setFixedHeight(50);


    // -- middle -- //
    QWidget* middleWidget = new QWidget;
    QHBoxLayout* middleLayout = new QHBoxLayout(middleWidget);

    // middle left
    QWidget* middleLeftWidget = new QWidget;
    QVBoxLayout* middleLeftLayout = new QVBoxLayout(middleLeftWidget);
    middleLeftWidget->setFixedWidth(250);
    middleLeftWidget->setStyleSheet("background-color:green");

    QPixmap cameraOnePlaceholder = QPixmap(":/UserApplicationModule/placeholder.jpeg");
    QLabel* cameraOneImageLabel = new QLabel();
    cameraOneImageLabel->setFixedWidth(230);
    cameraOneImageLabel->setPixmap(cameraOnePlaceholder.scaled(cameraOneImageLabel->width(), cameraOneImageLabel->height(), Qt::KeepAspectRatio));
    middleLeftLayout->addWidget(cameraOneImageLabel);
    QLabel* cameraOneTextLabel = new QLabel("Camera one");
    middleLeftLayout->addWidget(cameraOneTextLabel);

    QPixmap cameraTwoPlaceholder = QPixmap(":/UserApplicationModule/placeholder.jpeg");
    QLabel* cameraTwoImageLabel = new QLabel();
    cameraTwoImageLabel->setFixedWidth(230);
    cameraTwoImageLabel->setPixmap(cameraTwoPlaceholder.scaled(cameraTwoImageLabel->width(), cameraTwoImageLabel->height(), Qt::KeepAspectRatio));
    middleLeftLayout->addWidget(cameraTwoImageLabel);
    QLabel* cameraTwoTextLabel = new QLabel("Camera two");
    middleLeftLayout->addWidget(cameraTwoTextLabel);

    // middle right
    QWidget* middleRightWidget = new QWidget;
    QVBoxLayout* middleRightLayout = new QVBoxLayout(middleRightWidget);
    middleRightWidget->setFixedWidth(450);
    middleRightWidget->setStyleSheet("background-color:darkGreen");

    QWidget* resultWidget = new QWidget;
    QGridLayout* resultLayout = new QGridLayout(resultWidget);
    resultWidget->setFixedWidth(430);
    resultWidget->setFixedHeight(300);
    resultWidget->setStyleSheet("background-color:white");
    middleRightLayout->addWidget(resultWidget, 0, Qt::AlignHCenter);

    QStringList items = (QStringList() << "FR" << "WP" << "WB" << "WN" << "WR" << "WQ" << "WK" << "BP" << "BB" << "BN" << "BR" << "BQ" << "BK");
    std::array<QComboBox*, 64> comboBoxes;
    int flat;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            flat = i + j * 8;
            comboBoxes[flat] = new QComboBox;
            comboBoxes[flat]->addItems(items);
            resultLayout->addWidget(comboBoxes[flat], i, j);
        }
    }

    QWidget* radioWidget = new QWidget;
    QHBoxLayout* radioLayout = new QHBoxLayout(radioWidget);
    radioWidget->setFixedHeight(50);
    radioWidget->setStyleSheet("background-color:magenta");
    QRadioButton* trainRadioButton = new QRadioButton("Train");
    QRadioButton* testRadioButton = new QRadioButton("Test");
    radioLayout->addWidget(trainRadioButton);
    radioLayout->addWidget(testRadioButton);
    middleRightLayout->addWidget(radioWidget);

    QWidget* actionWidget = new QWidget;
    QHBoxLayout* actionLayout = new QHBoxLayout(actionWidget);
    actionWidget->setFixedHeight(50);
    actionWidget->setStyleSheet("background-color:darkRed");
    QPushButton* runButton = new QPushButton("Run KNN");
    QPushButton* resetButton = new QPushButton("Reset KNN");
    actionLayout->addWidget(runButton);
    actionLayout->addWidget(resetButton);
    middleRightLayout->addWidget(actionWidget);

    middleLayout->addWidget(middleLeftWidget);
    middleLayout->addWidget(middleRightWidget);
    middleWidget->setStyleSheet("background-color:blue");
    middleWidget->setFixedHeight(500);

    // -- message -- //
    QWidget* messageWidget = new QWidget;
    QHBoxLayout* messageLayout = new QHBoxLayout(messageWidget);
    QLabel* messageLabel = new QLabel("message");
    messageLayout->addWidget(messageLabel);
    messageWidget->setStyleSheet("background-color:cyan");
    messageWidget->setFixedHeight(50);

    // -- bottom -- //
    QWidget* bottomWidget = new QWidget;
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWidget);
    QPushButton* getImageButton = new QPushButton("Get image");
    QPushButton* sendToVARButton = new QPushButton("Send to VAR");
    QPushButton* helpButton = new QPushButton("Help");
    QPushButton* exitButton = new QPushButton("Exit");
    bottomLayout->addWidget(getImageButton);
    bottomLayout->addWidget(sendToVARButton);
    bottomLayout->addWidget(helpButton);
    bottomLayout->addWidget(exitButton);
    bottomWidget->setStyleSheet("background-color:gray");
    bottomWidget->setFixedHeight(50);

    centralLayout->addWidget(topWidget);
    centralLayout->addWidget(middleWidget);
    centralLayout->addWidget(messageWidget);
    centralLayout->addWidget(bottomWidget);

    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);
    //ui.setupUi(this);
}

UserApplicationModule::~UserApplicationModule()
{}

