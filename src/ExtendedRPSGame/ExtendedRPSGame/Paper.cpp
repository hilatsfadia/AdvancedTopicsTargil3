#include "stdafx.h"
#include "Paper.h"

Piece* Paper::FightWithOtherPieceType(Piece* enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy->GetPieceType();

	switch (enemyPieceType)
	{
		case (PieceFactory::PieceType::Rock):
		{
			return enemy->LoseToPiece(this);
			break;
		}
		case (PieceFactory::PieceType::Scissors):
		{
			return LoseToPiece(enemy);
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

bool Paper::IsStrongerThan(Piece* other) const
{
	PieceFactory::PieceType otherPieceType = other->GetPieceType();
	if ((otherPieceType == PieceFactory::PieceType::Flag) || (otherPieceType == PieceFactory::PieceType::Rock))
	{
		return true;
	}

	return false;
}
