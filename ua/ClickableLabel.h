#pragma once

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QMouseEvent>
#include <QPixmap>

// https://wiki.qt.io/Clickable_QLabel
class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();
    int row;
    int col;
    void setPiece(std::string pieceName, QPixmap piecePixmap);
    std::string getPieceName();

signals:
    void leftClicked(int row, int col, std::string pieceName);
    void rightClicked(int row, int col, std::string pieceName);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    std::string pieceName;
};
