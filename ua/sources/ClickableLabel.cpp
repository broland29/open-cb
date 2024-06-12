#include "../headers/ClickableLabel.h"

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

void ClickableLabel::setPiece(QString pieceName, QPixmap piecePixmap)
{
    this->pieceName = pieceName;
    this->setPixmap(piecePixmap);
}

QString ClickableLabel::getPieceName()
{
    if (pieceName == "FR")
    {
        if ((row + col) % 2 == 0)
        {
            return "WF";
        }
        return "BF";
    }
    return pieceName;
}

void ClickableLabel::modifyStyleSheet(QString string)
{
    if ((row + col) % 2 == 0)
    {
        setStyleSheet("background-color:" + Style::LIGHTEST + "; " + string);
    }
    else
    {
        setStyleSheet("background-color:" + Style::DARKEST + "; " + string);
    }
}