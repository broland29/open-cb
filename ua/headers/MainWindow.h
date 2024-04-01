#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include "ClickableLabel.h"
#include "EncodingMapper.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#if FMT_VERSION >= 90000
// https://github.com/fmtlib/fmt/issues/2245
template<> struct fmt::formatter<QString> : formatter<const char*> {
    auto format(const QString& s, format_context& ctx) {
        return formatter<const char*>::format((const char*)s.toUtf8(), ctx);
    }
};
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent* event);
private:
    QLabel* cameraOneImageLabel;
    QLabel* cameraTwoImageLabel;

    std::array<std::array<ClickableLabel*, 8>, 8> pieceLabels;
    
    QPushButton* validateMoveButton;
    QPushButton* discardMoveButton;
    QPushButton* newGameButton;

    QComboBox* classifierComboBox;
    QPushButton* saveClassifierButton;
    QPushButton* loadClassifierButton;
    QPushButton* trainClassifierButton;
    QPushButton* testClassifierButton;
    QPushButton* classifyBoardButton;


    QLabel* messageLabel;


    QPushButton* testConfigureButton;
    QPushButton* configureButton;
    QPushButton* testCropAndLabelButton;
    QPushButton* cropAndLabelButton;
    QPushButton* shuffleAndSplitButton;
    QPushButton* clearAllImagesButton;
    QPushButton* settingsButton;
    QPushButton* helpButton;


    // clicking logic
    std::array<std::string, 13> pieceNames = { "FR", "WP", "WB", "WN", "WR", "WQ", "WK", "BP", "BB", "BN", "BR", "BQ", "BK" };
    std::map<std::string, QPixmap> nameToPixmap;
    int lastRow = -1;
    int lastCol = -1;
    std::string lastPieceName;

    void setInitialSetup();
    QString getBoardFromChessGUI();

public slots:
    // slots for raw qt signals
    void validateMoveButtonClicked();
    void discardMoveButtonClicked();
    void newGameButtonClicked();

    void classifierComboBoxChanged();
    void saveClassifierButtonClicked();
    void loadClassifierButtonClicked();
    void trainClassifierButtonClicked();
    void testClassifierButtonClicked();
    void classifyBoardButtonClicked();

    void testConfigureButtonClicked();
    void configureButtonClicked();
    void testCropAndLabelButtonClicked();
    void cropAndLabelButtonClicked();
    void shuffleAndSplitButtonClicked();
    void clearAllImagesButtonClicked();
    void settingsButtonClicked();
    void helpButtonClicked();

    // reply slots for signals
    void validateMoveReplySlot(bool succeeded, QString message);
    void discardMoveReplySlot(bool succeeded, QString message);
    void newGameReplySlot(bool succeeded, QString message);

    void changeClassifierReplySlot(bool succeeded, QString message);
    void saveClassifierReplySlot(bool succeeded, QString message);
    void loadClassifierReplySlot(bool succeeded, QString message);
    void trainClassifierReplySlot(bool succeeded, QString message);
    void testClassifierReplySlot(bool succeeded, QString message);
    void classifyBoardReplySlot(bool succeeded, QString message);

    void testConfigureReplySlot(bool succeeded, QString message);
    void configureReplySlot(bool succeeded, QString message);
    void testCropAndLabelReplySlot(bool succeeded, QString message);
    void cropAndLabelReplySlot(bool succeeded, QString message);
    void shuffleAndSplitReplySlot(bool succeeded, QString message);
    void clearAllImagesReplySlot(bool succeeded, QString message);
    void changeSettingsReplySlot(bool succeeded, QString message);

    // clicks on chess GUI
    void leftClickedSlot(int row, int col, std::string pieceName);
    void rightClickedSlot(int row, int col, std::string pieceName);

    // getting a new frame
    void previewImageReadySlotLeft(QImage previewImage);
    void previewImageReadySlotRight(QImage previewImage);

signals:
    void validateMoveSignal(QString board);
    void discardMoveSignal();
    void newGameSignal();

    void changeClassifierSignal(QString classifier);
    void saveClassifierSignal(QString path);
    void loadClassifierSignal(QString path);
    void trainClassifierSignal();
    void testClassifierSignal();
    void classifyBoardSignal();

    void testConfigureSignal();
    void configureSignal();
    void testCropAndLabelSignal(QString board);
    void cropAndLabelSignal(QString board);
    void shuffleAndSplitSignal();
    void clearAllImagesSignal();
    void changeSettingsSignal();
};
