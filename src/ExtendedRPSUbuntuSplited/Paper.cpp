#include "Paper.h"

using namespace HilaAndJaelExtendedRPS;

Piece::WinningPiece Paper::FightWithOtherPieceType(Piece& enemy)
{
	PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceType::Rock):
		{
			enemy.LoseToPiece();
			return WinningPiece::CallingObject;
			break;
		}
		case (PieceType::Scissors):
		{
			LoseToPiece();
			return WinningPiece::OtherObject;
			break;
		}
		default:
		{
			return RPSPiece::FightWithOtherPieceType(enemy);
		}
	}
}

// TODO: think about joker...
bool Paper::IsStrongerThan(const Piece& other) const
{
	PieceType otherPieceType = other.GetPieceType();
	if ((otherPieceType == PieceType::Flag) || (otherPieceType == PieceType::Rock))
	{
		return true;
	}
	else
	{
		return RPSPiece::IsStrongerThan(other);
	}
}
