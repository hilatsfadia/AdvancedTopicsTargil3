#include "stdafx.h"
#include "Scissors.h"

Piece* Scissors::FightWithOtherPieceType(Piece* enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy->GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceFactory::PieceType::Rock):
		{
			return LoseToPiece(enemy);
			break;
		}
		case (PieceFactory::PieceType::Paper):
		{
			return enemy->LoseToPiece(this);
			break;
		}
		case (PieceFactory::PieceType::Bomb):
		{
			return BothPiecesLosers(enemy);
			break;
		}
		case (PieceFactory::PieceType::Joker):
		{
			return enemy->Fight(this);
			break;
		}
		case (PieceFactory::PieceType::Flag):
		{
			return enemy->LoseToPiece(this);
			break;
		}
		default:
		{
			return nullptr;
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
