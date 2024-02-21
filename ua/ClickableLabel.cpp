#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent)
{
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        qDebug() << "right click " << row << col;
        emit rightClicked(row, col, pieceName);
    }
    else if (event->button() == Qt::LeftButton)
    {
        qDebug() << "left click" << row << col;
        emit leftClicked(row, col, pieceName);
    }
    else
    {
        qDebug() << "Unrecognized QMouseEvent button" << event->button();
    }
}

void ClickableLabel::setPiece(std::string pieceName, QPixmap piecePixmap)
{
    this->pieceName = pieceName;
    this->setPixmap(piecePixmap);
}

std::string ClickableLabel::getPieceName()
{
    return pieceName;
}

void ClickableLabel::modifyStyleSheet(QString string)
{
    if ((row + col) % 2 == 0)
    {
        setStyleSheet("background-color:mistyrose; " + string);
    }
    else
    {
        setStyleSheet("background-color:brown; " + string);
    }
}