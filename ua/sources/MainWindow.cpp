#include "../headers/MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    setWindowTitle("OpenCB");
    setFixedSize(800, 700);

    QWidget* centralWidget = new QWidget;
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
    middleLeftWidget->setFixedWidth(300);
    middleLeftWidget->setStyleSheet("background-color:green");

    QPixmap cameraOnePlaceholder = QPixmap(":/UserApplicationModule/placeholder.jpeg");
    cameraOneImageLabel = new QLabel();
    //cameraOneImageLabel->setFixedWidth(230);
    cameraOneImageLabel->setFixedHeight(200);
    //cameraOneImageLabel->adjustSize();
    //cameraOneImageLabel->setMinimumWidth(320);
    cameraOneImageLabel->setScaledContents(true);
    //cameraOneImageLabel->setPixmap(cameraOnePlaceholder.scaled(cameraOneImageLabel->width(), cameraOneImageLabel->height(), Qt::KeepAspectRatio));
    middleLeftLayout->addWidget(cameraOneImageLabel);
    QLabel* cameraOneTextLabel = new QLabel("Camera one - left");
    middleLeftLayout->addWidget(cameraOneTextLabel);

    QPixmap cameraTwoPlaceholder = QPixmap(":/UserApplicationModule/placeholder.jpeg");
    cameraTwoImageLabel = new QLabel();
    //cameraTwoImageLabel->setFixedWidth(230);
    cameraTwoImageLabel->setFixedHeight(200);
    cameraTwoImageLabel->setScaledContents(true);
    //cameraTwoImageLabel->setPixmap(cameraTwoPlaceholder.scaled(cameraTwoImageLabel->width(), cameraTwoImageLabel->height(), Qt::KeepAspectRatio));
    middleLeftLayout->addWidget(cameraTwoImageLabel);
    QLabel* cameraTwoTextLabel = new QLabel("Camera two - right");
    middleLeftLayout->addWidget(cameraTwoTextLabel);

    // middle right
    QWidget* middleRightWidget = new QWidget;
    QVBoxLayout* middleRightLayout = new QVBoxLayout(middleRightWidget);
    middleRightWidget->setFixedWidth(450);
    middleRightWidget->setStyleSheet("background-color:darkGreen");

    QWidget* resultWidget = new QWidget;
    resultWidget->setStyleSheet("background-color:white");
    QGridLayout* resultLayout = new QGridLayout(resultWidget);
    resultWidget->setFixedWidth(300);
    resultWidget->setFixedHeight(300);
    resultLayout->setSpacing(0);

    int flat;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            pieceLabels[i][j] = new ClickableLabel();
            pieceLabels[i][j]->row = i;
            pieceLabels[i][j]->col = j;
            pieceLabels[i][j]->modifyStyleSheet("border: 2px solid black;");
            connect(pieceLabels[i][j], &ClickableLabel::leftClicked, this, &MainWindow::leftClickedSlot);
            connect(pieceLabels[i][j], &ClickableLabel::rightClicked, this, &MainWindow::rightClickedSlot);
            resultLayout->addWidget(pieceLabels[i][j], i, j);
        }
    }

    int w = 30;
    int h = 30;
    qDebug() << w << h;
    for (auto pieceName : pieceNames)
    {
        char resourcePath[256];
        sprintf(resourcePath, ":pieceImages/%s.png", pieceName.c_str());
        QPixmap piecePixmap;
        if (!piecePixmap.load(resourcePath))
        {
            qDebug() << "Falied to load" << resourcePath;
        }
        else
        {
            qDebug() << "Succesfully loaded" << resourcePath;
        }
        piecePixmap = piecePixmap.scaled(w, h, Qt::KeepAspectRatio);
        nameToPixmap.insert(std::pair(pieceName, piecePixmap));
    }

    _setInitialSetup();
    middleRightLayout->addWidget(resultWidget, 0, Qt::AlignHCenter);

    QWidget* actionWidget = new QWidget;
    QGridLayout* actionLayout = new QGridLayout(actionWidget);
    actionWidget->setFixedHeight(130);
    actionWidget->setStyleSheet("background-color:darkRed");
    sendToTrainButton = new QPushButton("Send to Train");
    sendToTestButton = new QPushButton("Send to Test");
    runTrainButton = new QPushButton("Run Train");
    runTestButton = new QPushButton("Run Test");
    resetTrainButton = new QPushButton("Reset Train");
    resetTestButton = new QPushButton("Reset Test");
    actionLayout->addWidget(sendToTrainButton, 0, 0);
    actionLayout->addWidget(sendToTestButton, 0, 1);
    actionLayout->addWidget(runTrainButton, 1, 0);
    actionLayout->addWidget(runTestButton, 1, 1);
    actionLayout->addWidget(resetTrainButton, 2, 0);
    actionLayout->addWidget(resetTestButton, 2, 1);

    QWidget* checkboxWidget = new QWidget;
    QHBoxLayout* checkboxLayout = new QHBoxLayout(checkboxWidget);
    classifyWhenGettingImageCheckbox = new QCheckBox("Classify when getting image");
    checkboxLayout->addWidget(classifyWhenGettingImageCheckbox, 0, Qt::AlignCenter);
    //checkboxWidget->setFixedHeight(50);

    middleRightLayout->addWidget(actionWidget);
    middleRightLayout->addWidget(checkboxWidget);

    middleLayout->addWidget(middleLeftWidget);
    middleLayout->addWidget(middleRightWidget);
    middleWidget->setStyleSheet("background-color:blue");
    middleWidget->setFixedHeight(500);

    // -- message -- //
    QWidget* messageWidget = new QWidget;
    QHBoxLayout* messageLayout = new QHBoxLayout(messageWidget);
    messageLabel = new QLabel("message");
    messageLayout->addWidget(messageLabel);
    messageWidget->setStyleSheet("background-color:cyan");
    messageWidget->setFixedHeight(50);

    // -- bottom -- //
    QWidget* bottomWidget = new QWidget;
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWidget);
    configureButton = new QPushButton("Configure");
    getImageButton = new QPushButton("Get image");
    sendToVARButton = new QPushButton("Send to VAR");
    getFromVARButton = new QPushButton("Get from VAR");
    newGameButton = new QPushButton("New game");
    helpButton = new QPushButton("Help");
    exitButton = new QPushButton("Exit");
    bottomLayout->addWidget(configureButton);
    bottomLayout->addWidget(getImageButton);
    bottomLayout->addWidget(sendToVARButton);
    bottomLayout->addWidget(getFromVARButton);
    bottomLayout->addWidget(newGameButton);
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

    connect(sendToTrainButton, SIGNAL(clicked()), this, SLOT(sendToTrainButtonClicked()));
    connect(sendToTestButton, SIGNAL(clicked()), this, SLOT(sendToTestButtonClicked()));
    connect(runTrainButton, SIGNAL(clicked()), this, SLOT(runTrainButtonClicked()));
    connect(runTestButton, SIGNAL(clicked()), this, SLOT(runTestButtonClicked()));
    connect(resetTrainButton, SIGNAL(clicked()), this, SLOT(resetTrainButtonClicked()));
    connect(resetTestButton, SIGNAL(clicked()), this, SLOT(resetTestButtonClicked()));

    connect(configureButton, &QPushButton::clicked, this, &MainWindow::configureButtonClicked);
    connect(getImageButton, &QPushButton::clicked, this, &MainWindow::getImageButtonClicked);
    connect(sendToVARButton, &QPushButton::clicked, this, &MainWindow::sendToVARButtonClicked);
    connect(getFromVARButton, &QPushButton::clicked, this, &MainWindow::getFromVARButtonClicked);
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::newGameButtonClicked);
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::helpButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitButtonClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::_setInitialSetup()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 2; j < 6; j++)
        {
            pieceLabels[j][i]->setPiece("FR", nameToPixmap["FR"]);
        }
    }
    pieceLabels[0][0]->setPiece("BR", nameToPixmap["BR"]);
    pieceLabels[0][1]->setPiece("BN", nameToPixmap["BN"]);
    pieceLabels[0][2]->setPiece("BB", nameToPixmap["BB"]);
    pieceLabels[0][3]->setPiece("BQ", nameToPixmap["BQ"]);
    pieceLabels[0][4]->setPiece("BK", nameToPixmap["BK"]);
    pieceLabels[0][5]->setPiece("BB", nameToPixmap["BB"]);
    pieceLabels[0][6]->setPiece("BN", nameToPixmap["BN"]);
    pieceLabels[0][7]->setPiece("BR", nameToPixmap["BR"]);
    for (int i = 0; i < 8; i++)
    {
        pieceLabels[1][i]->setPiece("BP", nameToPixmap["BP"]);
        pieceLabels[6][i]->setPiece("WP", nameToPixmap["WP"]);
    }
    pieceLabels[7][0]->setPiece("WR", nameToPixmap["WR"]);
    pieceLabels[7][1]->setPiece("WN", nameToPixmap["WN"]);
    pieceLabels[7][2]->setPiece("WB", nameToPixmap["WB"]);
    pieceLabels[7][3]->setPiece("WQ", nameToPixmap["WQ"]);
    pieceLabels[7][4]->setPiece("WK", nameToPixmap["WK"]);
    pieceLabels[7][5]->setPiece("WB", nameToPixmap["WB"]);
    pieceLabels[7][6]->setPiece("WN", nameToPixmap["WN"]);
    pieceLabels[7][7]->setPiece("WR", nameToPixmap["WR"]);
}

QString MainWindow::_extractComboBoxes()
{
    QString board;
    board.resize(64);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::string enc = pieceLabels[i][j]->getPieceName();

            char c;
            EncodingMapper::map(enc, c);
            board[i * 8 + j] = c;
        }
    }
    return board;
}


// ---------- right buttons, clicked slots ---------- //

void MainWindow::sendToTrainButtonClicked()
{
    QString board = _extractComboBoxes();
    messageLabel->setText(board);
    emit sendToTrainSignal(board);
}

void MainWindow::sendToTestButtonClicked()
{
    QString board = _extractComboBoxes();
    messageLabel->setText(board);
    emit sendToTestSignal(board);
}

void MainWindow::runTrainButtonClicked()
{
    messageLabel->setText("runTrainButtonClicked");
    emit runTrainSignal();
}

void MainWindow::runTestButtonClicked()
{
    messageLabel->setText("runTestButtonClicked");
    emit runTestSignal();
}

void MainWindow::resetTrainButtonClicked()
{
    messageLabel->setText("resetTrainButtonClicked");
    emit resetTrainSignal();
}

void MainWindow::resetTestButtonClicked()
{
    messageLabel->setText("resetTestButtonClicked");
    emit resetTestSignal();
}


// ---------- bottom buttons, clicked slots ---------- //

void MainWindow::configureButtonClicked()
{
    messageLabel->setText("configureButtonClicked");
    emit configureSignal();
}

void MainWindow::getImageButtonClicked()
{
    messageLabel->setText("getImageButtonClicked");
    emit getImageSignal(classifyWhenGettingImageCheckbox->isChecked());
}

void MainWindow::sendToVARButtonClicked()
{
    QString board = _extractComboBoxes();
    messageLabel->setText(board);
    emit sendToVARSignal(board);
}

void MainWindow::getFromVARButtonClicked()
{
    messageLabel->setText("getFromVARButtonClicked");
    emit getFromVARSignal();
}

void MainWindow::newGameButtonClicked()
{
    messageLabel->setText("newGameButtonClicked");
    emit newGameSignal();
}

void MainWindow::helpButtonClicked()
{
    messageLabel->setText("Help not implemented");
}

void MainWindow::exitButtonClicked()
{
    messageLabel->setText("exitButtonClicked");
    emit exitSignal();
}


// ---------- right buttons, IP -> UA ---------- //

void MainWindow::sendToTrainReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::sendToTestReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::runTrainReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::runTestReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::resetTrainReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::resetTestReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}


// ---------- bottom buttons, IP/VAR -> UA ---------- //
#define PATH_IMG_CAM_ONE "preview\\cam1.jpeg"
#define PATH_IMG_CAM_TWO "preview\\cam2.jpeg"

void MainWindow::configureReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::getImageReplySlot(bool succeeded, QString message)
{
    if (succeeded && message.at(0) == '$')  // we are getting a board
    {
        messageLabel->setText("Got board: " + message);

        for (int i = 1; i < 65; i++)
        {
            std::string encoding;
            EncodingMapper::map(message.at(i), encoding);
            pieceLabels[i / 8][i % 8]->setPiece(encoding, nameToPixmap[encoding]);
        }
        return;
    }

    messageLabel->setText(message);
}

void MainWindow::sendToVARReplySlot(QString message)
{
    messageLabel->setText(message);
}

void MainWindow::getFromVARReplySlot(QString board)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::string encoding;
            EncodingMapper::map(board[i * 8 + j], encoding);
            pieceLabels[i][j]->setPiece(encoding, nameToPixmap[encoding]);
        }
    }
}

void MainWindow::newGameReplySlot(bool succeeded)
{
    QString message = (succeeded) ? "Succeeded" : "Failed";
    messageLabel->setText(message);

    _setInitialSetup();
}

void MainWindow::exitReplySlot(bool succeeded)
{
    QString message = (succeeded) ? "Succeeded" : "Failed";
    messageLabel->setText(message);
}

// ---------- clicks on chess GUI ---------- //
void MainWindow::leftClickedSlot(int row, int col, std::string pieceName)
{
    qDebug() << "leftClickedSlot" << row << col;
    if (lastRow == -1 || lastCol == -1)
    {
        pieceLabels[row][col]->modifyStyleSheet("border: 2px solid red;");
        lastRow = row;
        lastCol = col;
        lastPieceName = pieceName;
        return;
    }

    pieceLabels[row][col]->setPiece(lastPieceName, nameToPixmap[lastPieceName]);
    pieceLabels[lastRow][lastCol]->setPiece("FR", nameToPixmap["FR"]);

    // reset selection
    pieceLabels[lastRow][lastCol]->modifyStyleSheet("border: 2px solid black;");
    lastRow = -1;
    lastCol = -1;
}

void MainWindow::rightClickedSlot(int row, int col, std::string pieceName)
{
    qDebug() << "rightClickedSlot" << row << col << pieceName;

    // reset selection
    lastRow = -1;
    lastCol = -1;

    int i;
    for (i = 0; i < 13; i++)
    {
        if (pieceNames[i] == pieceName)
        {
            break;
        }
    }
    if (i == 13)
    {
        qDebug() << "Piece name not found: " << pieceName;
        return;
    }
    std::string nextPieceName = pieceNames[(i + 1) % 13];
    qDebug() << "Changing to" << nextPieceName;
    pieceLabels[row][col]->setPiece(nextPieceName, nameToPixmap[nextPieceName]);
}


// ---------- getting a new frame ---------- // 
void MainWindow::imageUpdateSlotOne(QImage image)
{   
    QPixmap pixmap = QPixmap::fromImage(image);
    //cameraOneImageLabel->setPixmap(pixmap.scaled(cameraOneImageLabel->width(), cameraOneImageLabel->height(), Qt::KeepAspectRatio));
    cameraOneImageLabel->setPixmap(pixmap);
}

void MainWindow::imageUpdateSlotTwo(QImage image)
{
    QPixmap pixmap = QPixmap::fromImage(image);
    cameraTwoImageLabel->setPixmap(pixmap.scaled(cameraTwoImageLabel->width(), cameraTwoImageLabel->height(), Qt::KeepAspectRatio));
}