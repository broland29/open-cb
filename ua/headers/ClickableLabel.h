#pragma once

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>

#include "Style.h"


class ClickableLabel : public QLabel  // https://wiki.qt.io/Clickable_QLabel
{
    Q_OBJECT

public:
    int row;
    int col;

private:
    QString pieceName;

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    
    ~ClickableLabel();
    
    void setPiece(QString pieceName, QPixmap piecePixmap);
    
    QString getPieceName();
    
    void modifyStyleSheet(QString string);

private:

protected:
    void mousePressEvent(QMouseEvent* event);

signals:
    void leftClicked(int row, int col, QString pieceName);
    void rightClicked(int row, int col, QString pieceName);
};
