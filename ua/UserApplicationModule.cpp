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
    resize(500, 400);

    QWidget* centralWidget = new QWidget;

    /*
    // add label
    QLabel* label = new QLabel("label text", this);
    label->setText("New text");
    //label->move(100, 100);
    label->setFont(QFont("Times", 15));
    label->setStyleSheet("color:red");
    label->setGeometry(100, 100, 300, 300);
    */

    /*
    // add image
    QLabel* label = new QLabel(this);
    label->setPixmap(QPixmap(":/UserApplicationModule/placeholder.jpeg"));
    label->setGeometry(20, 20, 400, 400);
    */

    /*
    // add gif
    QLabel *label = new QLabel(this);
    QMovie* movie = new QMovie(":/UserApplicationModule/sun.gif");
    label->setGeometry(20, 20, 400, 400);
    label->setMovie(movie);
    movie->start();
    */

    /*
    // button
    QPushButton* button = new QPushButton(this);
    button->setText("Click me");
    button->setGeometry(30, 30, 130, 130);
    button->setFont(QFont("Times", 15));
    button->setIcon(QIcon(":/UserApplicationModule/sun.gif"));
    button->setIconSize(QSize(36, 36));
    */

    /*
    // add popup menu
    QMenu* menu = new QMenu();
    menu->setFont(QFont("Sanserif", 14));
    menu->setStyleSheet("background-color:yellow");
    menu->addAction("Copy");
    menu->addAction("Cut");
    menu->addAction("Paste");
    button->setMenu(menu);
    */

    /*
    // line edit
    QLineEdit* lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(20, 20, 250, 50);
    lineEdit->setFont(QFont("Times", 14));
    //lineEdit->setEnabled(false);
    lineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    */

    /*
    // horizontal box layout
    QPushButton* b1 = new QPushButton(centralWidget);
    b1->setText("b1");
    QPushButton* b2 = new QPushButton(centralWidget);
    b2->setText("b2");
    QPushButton* b3 = new QPushButton(centralWidget);
    b3->setText("b3");
    QPushButton* b4 = new QPushButton(centralWidget);
    b4->setText("b4");

    QHBoxLayout* hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(b1);
    hBoxLayout->addWidget(b2);
    hBoxLayout->addWidget(b3);
    hBoxLayout->addWidget(b4);
    centralWidget->setLayout(hBoxLayout);
    */

    /*
    // vertical box layout
    QPushButton* b1 = new QPushButton(centralWidget);
    b1->setText("b1");
    QPushButton* b2 = new QPushButton(centralWidget);
    b2->setText("b2");
    QPushButton* b3 = new QPushButton(centralWidget);
    b3->setText("b3");
    QPushButton* b4 = new QPushButton(centralWidget);
    b4->setText("b4");

    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(b1);
    vBoxLayout->addWidget(b2);
    vBoxLayout->addWidget(b3);
    vBoxLayout->addWidget(b4);
    centralWidget->setLayout(vBoxLayout);
    */

    /*
    // grid layout
    QPushButton* b1 = new QPushButton("b1", centralWidget);
    QPushButton* b2 = new QPushButton("b2", centralWidget);
    QPushButton* b3 = new QPushButton("b3", centralWidget);
    QPushButton* b4 = new QPushButton("b4", centralWidget);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(b1, 0, 0);
    gridLayout->addWidget(b2, 0, 1);
    gridLayout->addWidget(b3, 1, 0);
    gridLayout->addWidget(b4, 1, 1);
    centralWidget->setLayout(gridLayout);
    */

    /*
    // signal = event, slot = actionListener
    QPushButton* b1 = new QPushButton("Change text", centralWidget);
    connect(b1, SIGNAL(clicked()), this, SLOT(ChangeText()));
    label = new QLabel("hello", centralWidget);
    QHBoxLayout* hBoxLayout = new QHBoxLayout(centralWidget);
    hBoxLayout->addWidget(b1);
    hBoxLayout->addWidget(label);
    */

    /*
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    label = new QLabel("Hello", this);
    radioButton1 = new QRadioButton("Python", centralWidget);
    radioButton2 = new QRadioButton("C++", centralWidget);
    radioButton3 = new QRadioButton("Java", centralWidget);

    connect(radioButton1, SIGNAL(toggled(bool)), this, SLOT(ChangeRadio()));
    connect(radioButton2, SIGNAL(toggled(bool)), this, SLOT(ChangeRadio()));
    connect(radioButton3, SIGNAL(toggled(bool)), this, SLOT(ChangeRadio()));
    // setFont, setIcon, setIconSize
    hBoxLayout->addWidget(radioButton1);
    hBoxLayout->addWidget(radioButton2);
    hBoxLayout->addWidget(radioButton3);

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(label);
    vBoxLayout->addLayout(hBoxLayout);

    centralWidget->setLayout(vBoxLayout);
    */

    /*
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    label = new QLabel("Hello", this);
    checkBox1 = new QCheckBox("Python", centralWidget);
    checkBox2 = new QCheckBox("C++", centralWidget);
    checkBox3 = new QCheckBox("Java", centralWidget);
    connect(checkBox1, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheck()));
    connect(checkBox2, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheck()));
    connect(checkBox3, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheck()));
    hBoxLayout->addWidget(checkBox1);
    hBoxLayout->addWidget(checkBox2);
    hBoxLayout->addWidget(checkBox3);
    // setText, setFont, setIcon, setIconSize

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(label);
    vBoxLayout->addLayout(hBoxLayout);
    centralWidget->setLayout(vBoxLayout);
    */

    /*
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    QLabel *label1 = new QLabel();
    label1->setText("laptop price:");
    priceLineEdit = new QLineEdit();
    totalLineEdit = new QLineEdit();
    spinBox = new QSpinBox();
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(ChangeSpin()));
    hBoxLayout->addWidget(label1);
    hBoxLayout->addWidget(priceLineEdit);
    hBoxLayout->addWidget(spinBox);
    hBoxLayout->addWidget(totalLineEdit);
    centralWidget->setLayout(hBoxLayout);
    */

    /*
    // a clock which shows real time, ticks each second
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    lcdNumber = new QLCDNumber();
    lcdNumber->setStyleSheet("background-color:yellow");
    QTimer* timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    hBoxLayout->addWidget(lcdNumber);
    centralWidget->setLayout(hBoxLayout);
    */

    /*
    // random number gen
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    lcdNumber = new QLCDNumber();
    lcdNumber->setStyleSheet("background-color:red");
    QPushButton *button = new QPushButton("Random Generator");
    connect(button, SIGNAL(clicked()), this, SLOT(randomGenerator()));
    vBoxLayout->addWidget(lcdNumber);
    vBoxLayout->addWidget(button);
    centralWidget->setLayout(vBoxLayout);
    */

    /*
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    QLabel* label1 = new QLabel("Select account type");
    hBoxLayout->addWidget(label1);
    comboBox = new QComboBox();
    comboBox->addItem("Current Account");
    comboBox->addItem("Deposit Account");
    comboBox->addItem("SavingAccount");
    connect(comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(comboChanged()));
    hBoxLayout->addWidget(comboBox);
    label = new QLabel("Hello");
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(label);
    vBoxLayout->addLayout(hBoxLayout);
    centralWidget->setLayout(vBoxLayout);
    */

    /*
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    slider = new QSlider();
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(5);
    slider->setMinimum(0);
    slider->setMaximum(100);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeSlider()));
    hBoxLayout->addWidget(slider);
    label = new QLabel("Hello");
    hBoxLayout->addWidget(label);
    centralWidget->setLayout(hBoxLayout);
    */

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    QListWidget* listWidget = new QListWidget();
    listWidget->insertItem(0, "C++");
    listWidget->insertItem(1, "Python");
    listWidget->insertItem(2, "Java");
    vBoxLayout->addWidget(listWidget);
    centralWidget->setLayout(vBoxLayout);

    // QFontComboBox
    // QTableWidget 2:42:43, QTableWidgetItem
    // QCalendarWidget 2:49:03
    // QInputDialog 2:56:32
    // QColorDialog 3:09:14
    // QFontDialog 3:14:41
    // QMessageBox 3:21:45

    setCentralWidget(centralWidget);
    //ui.setupUi(this);
}

void UserApplicationModule::ChangeText()
{
    label->setText("This is changed");
}

UserApplicationModule::~UserApplicationModule()
{}

void UserApplicationModule::ChangeRadio()
{
    if (radioButton1->isChecked())
    {
        label->setText("Python");
    }
    if (radioButton2->isChecked())
    {
        label->setText("C++");
    }
    if (radioButton3->isChecked())
    {
        label->setText("Java");
    }
}

void UserApplicationModule::ChangeCheck()
{
    QString value = "";
    if (checkBox1->isChecked())
    {
        value = checkBox1->text();
    }
    if (checkBox2->isChecked())
    {
        value = checkBox2->text();
    }
    if (checkBox3->isChecked())
    {
        value = checkBox3->text();
    }
    label->setText("You have selected " + value);
}

void UserApplicationModule::ChangeSpin()
{
    int myPrice = priceLineEdit->text().toInt();
    int totalNumber = spinBox->value() * myPrice;
    totalLineEdit->setText(QString::number(totalNumber));
}

void UserApplicationModule::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if (time.second() % 2 == 0)
    {
        text[2] = ' ';
    }
    lcdNumber->display(text);
}

void UserApplicationModule::randomGenerator()
{
    int number = rand();
    lcdNumber->display(number);
}

void UserApplicationModule::comboChanged()
{
    QString item = comboBox->currentText();
    label->setText("Your account type is " + item);
}

void UserApplicationModule::changeSlider()
{
    label->setText(QString::number(slider->value()));
}
