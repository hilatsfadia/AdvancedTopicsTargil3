#include "stdafx.h"
#include "Rock.h"

Piece::WinningPiece Rock::FightWithOtherPieceType(Piece& enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceFactory::PieceType::Paper):
		{
			LoseToPiece();
			return WinningPiece::enemy;
			break;
		}
		case (PieceFactory::PieceType::Scissors):
		{
			enemy.LoseToPiece();
			return WinningPiece::ThisPiece;
			break;
		}
		default:
		{
			return RPSPiece::FightWithOtherPieceType(enemy);
		}
	}
}

bool Rock::IsStrongerThan(Piece* other) const
{
	PieceFactory::PieceType otherPieceType = other->GetPieceType();
	if ((otherPieceType == PieceFactory::PieceType::Flag) || (otherPieceType == PieceFactory::PieceType::Scissors))
	{
		return true;
	}

	return false;
}