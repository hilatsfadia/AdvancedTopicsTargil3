#include "stdafx.h"
#include "Rock.h"

Piece::WinningPiece Rock::FightWithOtherPieceType(Piece& enemy)
{
	PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceType::Paper):
		{
			LoseToPiece();
			return WinningPiece::OtherObject;
			break;
		}
		case (PieceType::Scissors):
		{
			enemy.LoseToPiece();
			return WinningPiece::CallingObject;
			break;
		}
		default:
		{
			return RPSPiece::FightWithOtherPieceType(enemy);
		}
	}
}

bool Rock::IsStrongerThan(const Piece& other) const
{
	PieceType otherPieceType = other.GetPieceType();
	if ((otherPieceType == PieceType::Flag) || (otherPieceType == PieceType::Scissors))
	{
		return true;
	}
	else
	{
		return RPSPiece::IsStrongerThan(other);
	}
}