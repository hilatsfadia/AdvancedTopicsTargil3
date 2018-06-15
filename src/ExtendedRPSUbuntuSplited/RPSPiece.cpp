#include "RPSPiece.h"
#include "Joker.h"

using namespace HilaAndJaelExtendedRPS;

// To be changed in inherired classes
Piece::WinningPiece RPSPiece::FightWithOtherPieceType(Piece & enemy)
{
	PieceType enemyPieceType = enemy.GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceType::Bomb):
		{
			BothPiecesLosers(enemy);
			return WinningPiece::Tie;
			break;
		}
		case (PieceType::Joker):
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
		case (PieceType::Flag):
		{
			enemy.LoseToPiece();
			return WinningPiece::CallingObject;
			break;
		}
		default:
		{
			return WinningPiece::Tie;
		}
	}
}

bool RPSPiece::IsStrongerThan(const Piece & other) const
{
	PieceType otherPieceType = other.GetPieceType();

	if (otherPieceType == PieceType::Joker)
	{
		const Joker& otherJoker = dynamic_cast<const Joker&>(other);
		return this->IsStrongerThan(*otherJoker.GetActualPiece()); // TODO: look
	}

	return false;
}
