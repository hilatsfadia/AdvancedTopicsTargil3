#include "stdafx.h"
#include "RPSPiece.h"
#include "Joker.h"

Piece::WinningPiece RPSPiece::FightWithOtherPieceType(Piece & enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceFactory::PieceType::Bomb):
		{
			BothPiecesLosers(enemy);
			return WinningPiece::Tie;
			break;
		}
		case (PieceFactory::PieceType::Joker):
		{
			if (enemy.Fight(*this) == WinningPiece::OtherObject)
			{
				return WinningPiece::CallingObject;
			}
			else if (enemy.Fight(*this) == WinningPiece::CallingObject)
			{
				return WinningPiece::OtherObject;
			}
			else
			{
				return WinningPiece::Tie;
			}
			break;
		}
		case (PieceFactory::PieceType::Flag):
		{
			enemy.LoseToPiece();
			return WinningPiece::CallingObject;
			break;
		}
		default:
		{
			// To be changed in inherired classes
			return WinningPiece::Tie;
		}
	}
}

bool RPSPiece::IsStrongerThan(const Piece & other) const
{
	PieceFactory::PieceType otherPieceType = other.GetPieceType();

	if (otherPieceType == PieceFactory::PieceType::Joker)
	{
		const Joker& otherJoker = dynamic_cast<const Joker&>(other);
		return this->IsStrongerThan(*otherJoker.GetActualPiece()); // TODO: look
	}

	return false;
}
