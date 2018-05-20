#include "stdafx.h"
#include "Rock.h"

Piece* Rock::FightWithOtherPieceType(Piece* enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy->GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceFactory::PieceType::Paper):
		{
			return LoseToPiece(enemy);
			break;
		}
		case (PieceFactory::PieceType::Scissors):
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

bool Rock::IsStrongerThan(Piece* other) const
{
	PieceFactory::PieceType otherPieceType = other->GetPieceType();
	if ((otherPieceType == PieceFactory::PieceType::Flag) || (otherPieceType == PieceFactory::PieceType::Scissors))
	{
		return true;
	}

	return false;
}