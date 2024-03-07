#include "../headers/Board.h"


std::ostream& operator<<(std::ostream& os, const Color& color)
{
    if (color == Color::WHITE)
    {
        os << "WHITE";
    }
    else if (color == Color::BLACK)
    {
        os << "BLACK";
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const Castle& castle)
{
    os << std::endl;
    os << "      didBlackKingMove: " << castle.didBlackKingMove << std::endl;
    os << "      didWhiteKingMove: " << castle.didWhiteKingMove << std::endl;
    os << "      didBlackKingsideRookMove: " << castle.didBlackKingsideRookMove << std::endl;
    os << "      didBlackQueensideRookMove: " << castle.didBlackQueensideRookMove << std::endl;
    os << "      didWhiteKingsideRookMove: " << castle.didWhiteKingsideRookMove << std::endl;
    os << "      didWhiteQueensideRookMove: " << castle.didWhiteQueensideRookMove << std::endl;
    return os;
}


std::ostream& operator<<(std::ostream& os, const Metadata& metadata)
{
    os << std::endl;
    os << "    caste: " << metadata.castle;
    os << "    enPassantCol: " << metadata.enPassantCol << std::endl;
    os << "    moveCount: " << metadata.moveCount << std::endl;
    os << "    turn: " << metadata.turn << std::endl;
    return os;
}


void Metadata::changeTurn()
{
    if (turn == Color::WHITE)
    {
        turn = Color::BLACK;
    }
    else
    {
        turn = Color::WHITE;
    }
}


void copyBoard(char boardTo[8][8], char boardFrom[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            boardTo[i][j] = boardFrom[i][j];
        }
    }
}
