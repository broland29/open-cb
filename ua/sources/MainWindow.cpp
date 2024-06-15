#include "../headers/MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    setWindowTitle("OpenCB");
    setFixedSize(800, 700);
    this->setStyleSheet("background-color:" + Style::LIGHT);

    QWidget* centralWidget = new QWidget;
    QVBoxLayout* centralLayout = new QVBoxLayout();


    QWidget* bigWidget = new QWidget;
    QHBoxLayout* bigLayout = new QHBoxLayout(bigWidget);
    bigWidget->setFixedHeight(500);
    bigWidget->setStyleSheet("background-color:" + Style::LIGHT);

    QWidget* bigLeftWidget = new QWidget;
    QVBoxLayout* middleLeftLayout = new QVBoxLayout(bigLeftWidget);
    bigLeftWidget->setFixedWidth(300);
    bigLeftWidget->setStyleSheet("background-color:" + Style::LIGHTEST);

    cameraOneImageLabel = new QLabel();
    cameraOneImageLabel->setFixedHeight(200);
    cameraOneImageLabel->setScaledContents(true);
    middleLeftLayout->addWidget(cameraOneImageLabel);
    QLabel* cameraOneTextLabel = new QLabel("Camera one - left");
    middleLeftLayout->addWidget(cameraOneTextLabel);

    QPixmap cameraTwoPlaceholder = QPixmap(":/UserApplicationModule/placeholder.jpeg");
    cameraTwoImageLabel = new QLabel();
    cameraTwoImageLabel->setFixedHeight(200);
    cameraTwoImageLabel->setScaledContents(true);
    middleLeftLayout->addWidget(cameraTwoImageLabel);
    QLabel* cameraTwoTextLabel = new QLabel("Camera two - right");
    middleLeftLayout->addWidget(cameraTwoTextLabel);

    bigLayout->addWidget(bigLeftWidget);


    QWidget* bigRightWidget = new QWidget;
    QVBoxLayout* bigRightLayout = new QVBoxLayout(bigRightWidget);
    bigRightWidget->setFixedWidth(450);
    bigRightWidget->setStyleSheet("background-color:" + Style::LIGHT);

    QWidget* chessGUIWidget = new QWidget;
    chessGUIWidget->setStyleSheet("background-color:" + Style::DARKEST);
    QGridLayout* chessGUILayout = new QGridLayout(chessGUIWidget);
    chessGUIWidget->setFixedWidth(300);
    chessGUIWidget->setFixedHeight(300);
    chessGUILayout->setSpacing(0);

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
            chessGUILayout->addWidget(pieceLabels[i][j], i, j);
        }
    }

    for (QString encoding : ENCODINGS)
    {
        QString resourcePath;
        if (encoding == "WF" || encoding == "BF")
        {
            resourcePath = ":pieceImages/FR.png";  // same empty png used for both frees
        }
        else
        {
            resourcePath = ":pieceImages/" + encoding + ".png";
        }

        QPixmap piecePixmap;
        if (!piecePixmap.load(resourcePath))
        {
            SPDLOG_ERROR("Falied to load {}!", resourcePath);
        }
        else
        {
            SPDLOG_TRACE("Succesfully loaded {}", resourcePath);
        }
        piecePixmap = piecePixmap.scaled(30, 30, Qt::KeepAspectRatio);
        nameToPixmap.insert(std::pair(encoding, piecePixmap));
    }

    setInitialSetup();
    bigRightLayout->addWidget(chessGUIWidget, 0, Qt::AlignHCenter);

    QWidget* VARWidget = new QWidget;
    QGridLayout* VARLayout = new QGridLayout(VARWidget);
    VARWidget->setFixedHeight(80);
    VARWidget->setStyleSheet("background-color:" + Style::LIGHT);
    validateMoveButton = new QPushButton("Validate Move");
    discardMoveButton = new QPushButton("Discard Move");
    openLogsButton = new QPushButton("Open Logs");
    newGameButton = new QPushButton("New Game");
    surrenderButton = new QPushButton("Surrender");
    drawButton = new QPushButton("Draw");
    validateMoveButton->setStyleSheet("background-color:" + Style::DARK);
    discardMoveButton->setStyleSheet("background-color:" + Style::DARK);
    openLogsButton->setStyleSheet("background-color:" + Style::DARK);
    newGameButton->setStyleSheet("background-color:" + Style::DARK);
    surrenderButton->setStyleSheet("background-color:" + Style::DARK);
    drawButton->setStyleSheet("background-color:" + Style::DARK);
    VARLayout->addWidget(validateMoveButton, 0, 0);
    VARLayout->addWidget(discardMoveButton, 0, 1);
    VARLayout->addWidget(openLogsButton, 0, 2);
    VARLayout->addWidget(newGameButton, 1, 0);
    VARLayout->addWidget(surrenderButton, 1, 1);
    VARLayout->addWidget(drawButton, 1, 2);
    bigRightLayout->addWidget(VARWidget);

    QWidget* classificationWidget = new QWidget;
    QGridLayout* classificationLayout = new QGridLayout(classificationWidget);
    classificationWidget->setFixedHeight(80);
    classificationWidget->setStyleSheet("background-color:" + Style::LIGHT);
    classifierComboBox = new QComboBox();
    classifierComboBox->addItem("KNN");
    classifierComboBox->addItem("SVM");
    classifierComboBox->addItem("CNN");
    saveClassifierButton = new QPushButton("Save");
    loadClassifierButton = new QPushButton("Load");
    trainClassifierButton = new QPushButton("Train");
    testClassifierButton = new QPushButton("Test");
    classifyBoardButton = new QPushButton("Classify Board");
    classifierComboBox->setStyleSheet("background-color:" + Style::DARK);
    saveClassifierButton->setStyleSheet("background-color:" + Style::DARK);
    loadClassifierButton->setStyleSheet("background-color:" + Style::DARK);
    trainClassifierButton->setStyleSheet("background-color:" + Style::DARK);
    testClassifierButton->setStyleSheet("background-color:" + Style::DARK);
    classifyBoardButton->setStyleSheet("background-color:" + Style::DARK);
    classificationLayout->addWidget(classifierComboBox, 0, 0);
    classificationLayout->addWidget(saveClassifierButton, 0, 1);
    classificationLayout->addWidget(loadClassifierButton, 0, 2);
    classificationLayout->addWidget(trainClassifierButton, 1, 0);
    classificationLayout->addWidget(testClassifierButton, 1, 1);
    classificationLayout->addWidget(classifyBoardButton, 1, 2);
    bigRightLayout->addWidget(classificationWidget);

    bigLayout->addWidget(bigRightWidget);

    centralLayout->addWidget(bigWidget);


    QWidget* messageWidget = new QWidget;
    QHBoxLayout* messageLayout = new QHBoxLayout(messageWidget);
    messageWidget->setFixedHeight(50);
    messageWidget->setStyleSheet("background-color:" + Style::LIGHTEST);
    messageLabel = new QLabel("Welcome to OpenCB!");
    messageLayout->addWidget(messageLabel);

    centralLayout->addWidget(messageWidget);


    QWidget* bottomWidget = new QWidget;
    QGridLayout* bottomLayout = new QGridLayout(bottomWidget);
    bottomWidget->setFixedHeight(80);
    //bottomWidget->setStyleSheet("background-color:" + Style::LIGHTEST);
    testConfigureButton = new QPushButton("Test Configure");
    configureButton = new QPushButton("Configure");
    testCropAndLabelButton = new QPushButton("Test Crop and Label");
    cropAndLabelButton = new QPushButton("Crop and Label");
    shuffleAndSplitButton = new QPushButton("Shuffle and Split");
    clearAllImagesButton = new QPushButton("Clear All Images");
    settingsButton = new QPushButton("Settings");
    helpButton = new QPushButton("Help");
    testConfigureButton->setStyleSheet("background-color:" + Style::DARK);
    configureButton->setStyleSheet("background-color:" + Style::DARK);
    testCropAndLabelButton->setStyleSheet("background-color:" + Style::DARK);
    cropAndLabelButton->setStyleSheet("background-color:" + Style::DARK);
    shuffleAndSplitButton->setStyleSheet("background-color:" + Style::DARK);
    clearAllImagesButton->setStyleSheet("background-color:" + Style::DARK);
    settingsButton->setStyleSheet("background-color:" + Style::DARK);
    helpButton->setStyleSheet("background-color:" + Style::DARK);
    bottomLayout->addWidget(testConfigureButton, 0, 0);
    bottomLayout->addWidget(configureButton, 1, 0);
    bottomLayout->addWidget(testCropAndLabelButton, 0, 1);
    bottomLayout->addWidget(cropAndLabelButton, 1, 1);
    bottomLayout->addWidget(shuffleAndSplitButton, 0, 2);
    bottomLayout->addWidget(clearAllImagesButton, 1, 2);
    bottomLayout->addWidget(settingsButton, 0, 3);
    bottomLayout->addWidget(helpButton, 1, 3);

    centralLayout->addWidget(bottomWidget);

    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);

    QObject::connect(validateMoveButton, &QPushButton::clicked, this, &MainWindow::validateMoveButtonClicked);
    QObject::connect(discardMoveButton, &QPushButton::clicked, this, &MainWindow::discardMoveButtonClicked);
    QObject::connect(openLogsButton, &QPushButton::clicked, this, &MainWindow::openLogsButtonClicked);
    QObject::connect(newGameButton, &QPushButton::clicked, this, &MainWindow::newGameButtonClicked);
    QObject::connect(surrenderButton, &QPushButton::clicked, this, &MainWindow::surrenderButtonClicked);
    QObject::connect(drawButton, &QPushButton::clicked, this, &MainWindow::offerDrawButtonClicked);

    QObject::connect(classifierComboBox, &QComboBox::currentTextChanged, this, &MainWindow::classifierComboBoxChanged);
    QObject::connect(saveClassifierButton, &QPushButton::clicked, this, &MainWindow::saveClassifierButtonClicked);
    QObject::connect(loadClassifierButton, &QPushButton::clicked, this, &MainWindow::loadClassifierButtonClicked);
    QObject::connect(trainClassifierButton, &QPushButton::clicked, this, &MainWindow::trainClassifierButtonClicked);
    QObject::connect(testClassifierButton, &QPushButton::clicked, this, &MainWindow::testClassifierButtonClicked);
    QObject::connect(classifyBoardButton, &QPushButton::clicked, this, &MainWindow::classifyBoardButtonClicked);

    QObject::connect(testConfigureButton, &QPushButton::clicked, this, &MainWindow::testConfigureButtonClicked);
    QObject::connect(configureButton, &QPushButton::clicked, this, &MainWindow::configureButtonClicked);
    QObject::connect(testCropAndLabelButton, &QPushButton::clicked, this, &MainWindow::testCropAndLabelButtonClicked);
    QObject::connect(cropAndLabelButton, &QPushButton::clicked, this, &MainWindow::cropAndLabelButtonClicked);
    QObject::connect(shuffleAndSplitButton, &QPushButton::clicked, this, &MainWindow::shuffleAndSplitButtonClicked);
    QObject::connect(clearAllImagesButton, &QPushButton::clicked, this, &MainWindow::clearAllImagesButtonClicked);
    QObject::connect(settingsButton, &QPushButton::clicked, this, &MainWindow::settingsButtonClicked);
    QObject::connect(helpButton, &QPushButton::clicked, this, &MainWindow::helpButtonClicked);
}

MainWindow::~MainWindow()
{
}

// https://stackoverflow.com/questions/17480984/how-do-i-handle-the-event-of-the-user-pressing-the-x-close-button
void MainWindow::closeEvent(QCloseEvent* event)
{
    //emit exitSignal();
    //event->ignore();
    // todo
}

void MainWindow::setInitialSetup()
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

QVector<QString> MainWindow::getEncodingsFromChessGUI()
{
    QVector<QString> encodings;
    encodings.resize(64);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            encodings[i * 8 + j] = pieceLabels[i][j]->getPieceName();
        }
    }
    return encodings;
}


// ---------- clicked/changed slots ---------- //

void MainWindow::validateMoveButtonClicked()
{
    QVector<QString> encodings = getEncodingsFromChessGUI();
    SPDLOG_TRACE("Emitting validateMoveSignal");
    emit validateMoveSignal(encodings);
}

void MainWindow::discardMoveButtonClicked()
{
    SPDLOG_TRACE("Emitting discardMoveSignal");
    emit discardMoveSignal();
}

void MainWindow::openLogsButtonClicked()
{
    SPDLOG_TRACE("Emitting discardMoveSignal");
    emit openLogsSignal();
}

void MainWindow::newGameButtonClicked()
{
    SPDLOG_TRACE("Emitting newGameSignal");
    emit newGameSignal();
}

void MainWindow::surrenderButtonClicked()
{
    SPDLOG_TRACE("Emitting surrenderSignal");
    emit surrenderSignal();
}

void MainWindow::offerDrawButtonClicked()
{
    SPDLOG_TRACE("Emitting offerDrawSignal");
    emit offerDrawSignal();
}



void MainWindow::classifierComboBoxChanged()
{
    QString classifierName = classifierComboBox->currentText();
    SPDLOG_TRACE("Emitting changeClassifierSignal with {}", classifierName);
    emit changeClassifierSignal(classifierName);
}

void MainWindow::saveClassifierButtonClicked()
{
    SPDLOG_TRACE("Emitting saveClassifierSignal");
    emit saveClassifierSignal();
}

void MainWindow::loadClassifierButtonClicked()
{
    SPDLOG_TRACE("Emitting loadClassifierSignal");
    emit loadClassifierSignal();
}

void MainWindow::trainClassifierButtonClicked()
{
    SPDLOG_TRACE("Emitting trainClassifierSignal");
    emit trainClassifierSignal();
}

void MainWindow::testClassifierButtonClicked()
{
    SPDLOG_TRACE("Emitting testClassifierSignal");
    emit testClassifierSignal();
}

void MainWindow::classifyBoardButtonClicked()
{
    SPDLOG_TRACE("Emitting classifyBoardSignal");
    emit classifyBoardSignal();
}



void MainWindow::testConfigureButtonClicked()
{
    SPDLOG_TRACE("Emitting testConfigureSignal");
    emit testConfigureSignal();
}

void MainWindow::configureButtonClicked()
{
    SPDLOG_TRACE("Emitting configureSignal");
    emit configureSignal();
}

void MainWindow::testCropAndLabelButtonClicked()
{
    QVector<QString> board = getEncodingsFromChessGUI();
    SPDLOG_TRACE("Emitting testCropAndLabelSignal");
    emit testCropAndLabelSignal(board);
}

void MainWindow::cropAndLabelButtonClicked()
{
    QVector<QString> board = getEncodingsFromChessGUI();
    SPDLOG_TRACE("Emitting cropAndLabelSignal");
    emit cropAndLabelSignal(board);
}

void MainWindow::shuffleAndSplitButtonClicked()
{
    SPDLOG_TRACE("Emitting shuffleAndSplitSignal");
    emit shuffleAndSplitSignal();
}

void MainWindow::clearAllImagesButtonClicked()
{
    SPDLOG_TRACE("Emitting clearAllImagesSignal");
    emit clearAllImagesSignal();
}



void MainWindow::settingsButtonClicked()
{
    SettingsDialog* settingsDialog = new SettingsDialog(this);

    // establish connections to new dialog
    QObject::connect(settingsDialog, &SettingsDialog::setParametersSignal, this, &MainWindow::setParametersSlot);
    QObject::connect(settingsDialog, &SettingsDialog::getParametersSignal, this, &MainWindow::getParametersSlot);
    QObject::connect(this, &MainWindow::setParametersReplySignal, settingsDialog, &SettingsDialog::setParametersReplySlot);
    QObject::connect(this, &MainWindow::getParametersReplySignal, settingsDialog, &SettingsDialog::getParametersReplySlot);

    getParametersSignal();

    int ret = settingsDialog->exec();
    SPDLOG_TRACE("Got return value {} from settingDialog.exec()", ret);
}

void MainWindow::helpButtonClicked()
{
    SPDLOG_TRACE("TODO - help");
}


// ---------- reply slots ---------- //

void MainWindow::validateMoveReplySlot(bool isValid, QString encoding, QString description)
{
    QString message;
    if (isValid)
    {
        message = "Valid move: " + encoding + " (" + description + ")";
    }
    else
    {
        message = "Invalid move: " + description;
    }
    messageLabel->setText(message);
}

void MainWindow::discardMoveReplySlot(bool succeeded, QString message, QVector<QString> encodings)
{
    messageLabel->setText(message);

    for (int i = 0; i < 64; i++)
    {
        pieceLabels[i / 8][i % 8]->setPiece(encodings[i], nameToPixmap[encodings[i]]);
    }
}

void MainWindow::openLogsReplySlot(bool succeeded, QString message)
{
    if (!succeeded)
    {
        messageLabel->setText(message);
    }
    // in success case, no need to show a message
}

void MainWindow::newGameReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
    if (succeeded)
    {
        setInitialSetup();
    }
}

void MainWindow::surrenderReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::offerDrawReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}



void MainWindow::changeClassifierReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::saveClassifierReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::loadClassifierReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::trainClassifierReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::testClassifierReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::classifyBoardReplySlot(bool succeeded, QString message, QVector<QString> encodings)
{
    if (!succeeded)
    {
        messageLabel->setText(message);
        return;
    }

    for (int i = 0; i < 64; i++)
    {
        pieceLabels[i / 8][i % 8]->setPiece(encodings[i], nameToPixmap[encodings[i]]);
    }

    messageLabel->setText(message);
}



void MainWindow::testConfigureReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::configureReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::testCropAndLabelReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::cropAndLabelReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::shuffleAndSplitReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::clearAllImagesReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}

void MainWindow::changeSettingsReplySlot(bool succeeded, QString message)
{
    messageLabel->setText(message);
}


// ---------- clicks on chess GUI ---------- //
void MainWindow::leftClickedSlot(int row, int col, QString pieceName)
{
    SPDLOG_TRACE("Left click ({},{})", row, col);
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

void MainWindow::rightClickedSlot(int row, int col, QString pieceName)
{
    SPDLOG_TRACE("Right click ({},{})", row, col);

    // reset selection
    lastRow = -1;
    lastCol = -1;

    // no WF/BF visible on board
    QVector<QString> switchOrder = { "FR", "WP", "WB", "WN", "WR", "WQ", "WK", "BP", "BB", "BN", "BR", "BQ", "BK" };
    int i;
    for (i = 0; i < switchOrder.size(); i++)
    {
        if (switchOrder[i] == pieceName)
        {
            goto _found;
        }
    }
    SPDLOG_ERROR("Piece name {} not found", pieceName);
    return;
    
_found:
    QString nextPieceName = switchOrder[(i + 1) % switchOrder.size()];
    SPDLOG_TRACE("Changing to {}", nextPieceName);
    pieceLabels[row][col]->setPiece(nextPieceName, nameToPixmap[nextPieceName]);
}


// ---------- getting a new frame ---------- // 
void MainWindow::previewImageReadySlotLeft(QImage previewImage)
{   
    QPixmap pixmap = QPixmap::fromImage(previewImage);
    //cameraOneImageLabel->setPixmap(pixmap.scaled(cameraOneImageLabel->width(), cameraOneImageLabel->height(), Qt::KeepAspectRatio));
    cameraOneImageLabel->setPixmap(pixmap);
}

void MainWindow::previewImageReadySlotRight(QImage previewImage)
{
    QPixmap pixmap = QPixmap::fromImage(previewImage);
    cameraTwoImageLabel->setPixmap(pixmap.scaled(cameraTwoImageLabel->width(), cameraTwoImageLabel->height(), Qt::KeepAspectRatio));
}


// ---------- delegating to dialog since inexistent at the beginning ---------- //
void MainWindow::setParametersReplySlot(bool succeeded, QString message)
{
    emit setParametersReplySignal(succeeded, message);
}

void MainWindow::getParametersReplySlot(Parameters parameters)
{
    emit getParametersReplySignal(parameters);
}

void MainWindow::setParametersSlot(Parameters parameters)
{
    emit setParametersSignal(parameters);
}

void MainWindow::getParametersSlot()
{
    emit getParametersSignal();
}
