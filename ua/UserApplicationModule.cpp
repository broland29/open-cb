#include "UserApplicationModule.h"
#include "EncodingMapper.h"

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
    cameraOneImageLabel = new QLabel();
    cameraOneImageLabel->setFixedWidth(230);
    cameraOneImageLabel->setPixmap(cameraOnePlaceholder.scaled(cameraOneImageLabel->width(), cameraOneImageLabel->height(), Qt::KeepAspectRatio));
    middleLeftLayout->addWidget(cameraOneImageLabel);
    QLabel* cameraOneTextLabel = new QLabel("Camera one");
    middleLeftLayout->addWidget(cameraOneTextLabel);

    QPixmap cameraTwoPlaceholder = QPixmap(":/UserApplicationModule/placeholder.jpeg");
    cameraTwoImageLabel = new QLabel();
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
            connect(pieceLabels[i][j], &ClickableLabel::leftClicked, this, &UserApplicationModule::leftClickedSlot);
            connect(pieceLabels[i][j], &ClickableLabel::rightClicked, this, &UserApplicationModule::rightClickedSlot);
            resultLayout->addWidget(pieceLabels[i][j], i, j);
        }
    }

    int w = 30;
    int h = 30;
    qDebug() << w << h;
    for (auto pieceName : pieceNames)
    {
        char resourcePath[256];
        sprintf(resourcePath, ":/UserApplicationModule/pieceImages/%s.png", pieceName.c_str());
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
    getImageButton = new QPushButton("Get image");
    sendToVARButton = new QPushButton("Send to VAR");
    getFromVARButton = new QPushButton("Get from VAR");
    helpButton = new QPushButton("Help");
    exitButton = new QPushButton("Exit");
    bottomLayout->addWidget(getImageButton);
    bottomLayout->addWidget(sendToVARButton);
    bottomLayout->addWidget(getFromVARButton);
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

    connect(getImageButton, SIGNAL(clicked()), this, SLOT(getImageButtonClicked()));
    connect(sendToVARButton, SIGNAL(clicked()), this, SLOT(sendToVARButtonClicked()));
    connect(getFromVARButton, SIGNAL(clicked()), this, SLOT(getFromVARButtonClicked()));
    connect(helpButton, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitButtonClicked()));
    //ui.setupUi(this);
}

UserApplicationModule::~UserApplicationModule()
{}

QString UserApplicationModule::_extractComboBoxes()
{
    QString board;
    board.resize(64);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::string enc = pieceLabels[i][j]->getPieceName();
            qDebug() << i << j << enc;

            char c;
            EncodingMapper::map(enc, c);
            board[i * 8 + j] = c;
        }
    }
    return board;
}

void UserApplicationModule::sendToTrainButtonClicked()
{
    QString board = _extractComboBoxes();
    messageLabel->setText(board);
    emit sendToTrainSignalIP(board);
}

void UserApplicationModule::sendToTestButtonClicked()
{
    QString board = _extractComboBoxes();
    messageLabel->setText(board);
    emit sendToTestSignalIP(board);
}

void UserApplicationModule::runTrainButtonClicked()
{
    messageLabel->setText("runTrainButtonClicked");
    emit runTrainSignalIP();
}

void UserApplicationModule::runTestButtonClicked()
{
    messageLabel->setText("runTestButtonClicked");
    emit runTestSignalIP();
}

void UserApplicationModule::resetTrainButtonClicked()
{
    messageLabel->setText("resetTrainButtonClicked");
    emit resetTrainSignalIP();
}

void UserApplicationModule::resetTestButtonClicked()
{
    messageLabel->setText("resetTestButtonClicked");
    emit resetTestSignalIP();
}

void UserApplicationModule::getImageButtonClicked()
{
    messageLabel->setText("getImageButtonClicked");
    emit requestImageSignalIP(classifyWhenGettingImageCheckbox->isChecked());
}

void UserApplicationModule::sendToVARButtonClicked()
{
    QString board = _extractComboBoxes();
    messageLabel->setText(board);
    emit sendToVARSignalVAR(board);
}

void UserApplicationModule::getFromVARButtonClicked()
{
    messageLabel->setText("getFromVARButtonClicked");
    emit getFromVARSignalVAR();
}

void UserApplicationModule::helpButtonClicked()
{
    messageLabel->setText("Help not implemented");
}

void UserApplicationModule::exitButtonClicked()
{
    emit exitSignalIP();
    emit exitSignalVAR();
    QCoreApplication::quit();
}

#define PATH_IMG_CAM_ONE "preview\\cam1.jpeg"
#define PATH_IMG_CAM_TWO "preview\\cam2.jpeg"

void UserApplicationModule::requestImageReplySlotIP(QString board)
{
    messageLabel->setText("Set image fired! Yippie!");

    QImage cameraOneImage, cameraTwoImage;
    if (cameraOneImage.load(PATH_IMG_CAM_ONE) == false)
    {
        messageLabel->setText("Could not load" + QString::fromUtf8(PATH_IMG_CAM_ONE));
        return;
    }
    if (cameraTwoImage.load(PATH_IMG_CAM_TWO) == false)
    {
        messageLabel->setText("Could not load" + QString::fromUtf8(PATH_IMG_CAM_TWO));
        return;
    }

    QPixmap cameraOnePixmap = QPixmap::fromImage(cameraOneImage);
    QPixmap cameraTwoPixmap = QPixmap::fromImage(cameraTwoImage);

    cameraOneImageLabel->setPixmap(cameraOnePixmap.scaled(cameraOneImageLabel->width(), cameraOneImageLabel->height(), Qt::KeepAspectRatio));
    cameraTwoImageLabel->setPixmap(cameraTwoPixmap.scaled(cameraTwoImageLabel->width(), cameraTwoImageLabel->height(), Qt::KeepAspectRatio));

    for (int i = 0; i < 64; i++)
    {   
        std::string encoding;
        EncodingMapper::map(board[i], encoding);
        pieceLabels[i / 8][i % 8]->setPiece(encoding, nameToPixmap[encoding]);
    }
}

void UserApplicationModule::sendToVARReplySlotVAR(QString message)
{
    messageLabel->setText(message);
}

void UserApplicationModule::getFromVARReplySlotVAR(QString board)
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

void UserApplicationModule::leftClickedSlot(int row, int col, std::string pieceName)
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

void UserApplicationModule::rightClickedSlot(int row, int col, std::string pieceName)
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