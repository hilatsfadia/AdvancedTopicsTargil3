#include "stdafx.h"
#include "Paper.h"

Piece::WinningPiece Paper::FightWithOtherPieceType(Piece& enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceFactory::PieceType::Rock):
		{
			enemy.LoseToPiece();
			return WinningPiece::ThisPiece;
			break;
		}
		case (PieceFactory::PieceType::Scissors):
		{
			LoseToPiece();
			return WinningPiece::enemy;
			break;
		}
		default:
		{
			return RPSPiece::FightWithOtherPieceType(enemy);
		}
	}
}

bool Paper::IsStrongerThan(Piece* other) const
{
	PieceFactory::PieceType otherPieceType = other->GetPieceType();
	if ((otherPieceType == PieceFactory::PieceType::Flag) || (otherPieceType == PieceFactory::PieceType::Rock))
	{
		return true;
	}

	return false;
}
