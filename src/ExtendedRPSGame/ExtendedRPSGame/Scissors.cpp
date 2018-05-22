#include "stdafx.h"
#include "Scissors.h"

Piece::WinningPiece Scissors::FightWithOtherPieceType(Piece& enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceFactory::PieceType::Rock):
		{
			LoseToPiece();
			return WinningPiece::OtherObject;
			break;
		}
		case (PieceFactory::PieceType::Paper):
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

bool Scissors::IsStrongerThan(Piece* other) const
{
	PieceFactory::PieceType otherPieceType = other->GetPieceType();
	if ((otherPieceType == PieceFactory::PieceType::Flag) || (otherPieceType == PieceFactory::PieceType::Paper))
	{
		return true;
	}

	return false;
}
