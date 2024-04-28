#pragma once

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>

// https://wiki.qt.io/Clickable_QLabel
class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();
    int row;
    int col;
    void setPiece(QString pieceName, QPixmap piecePixmap);
    QString getPieceName();
    void modifyStyleSheet(QString string);

signals:
    void leftClicked(int row, int col, QString pieceName);
    void rightClicked(int row, int col, QString pieceName);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QString pieceName;
};
