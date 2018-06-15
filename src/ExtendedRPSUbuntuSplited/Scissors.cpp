#include "Scissors.h"

using namespace HilaAndJaelExtendedRPS;

Piece::WinningPiece Scissors::FightWithOtherPieceType(Piece& enemy)
{
	PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceType::Rock):
		{
			LoseToPiece();
			return WinningPiece::OtherObject;
			break;
		}
		case (PieceType::Paper):
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

bool Scissors::IsStrongerThan(const Piece& other) const
{
	PieceType otherPieceType = other.GetPieceType();
	if ((otherPieceType == PieceType::Flag) || (otherPieceType == PieceType::Paper))
	{
		return true;
	}
	else
	{
		return RPSPiece::IsStrongerThan(other);
	}
}
