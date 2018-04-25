#include "stdafx.h"
#include "Rock.h"

Rock::~Rock()
{
}

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

	//if (Rock* rock = dynamic_cast<Rock*>(enemy))
	//{

	//}
	//else if (Paper* rock = dynamic_cast<Paper*>(enemy))
	//{

	//}
	//else if (Scissors* rock = dynamic_cast<Scissors*>(enemy))
	//{

	//}
	//else if (Bomb* rock = dynamic_cast<Bomb*>(enemy))
	//{

	//}
	//else if (Joker* rock = dynamic_cast<Joker*>(enemy))
	//{

	//}
	//else if (Flag* rock = dynamic_cast<Flag*>(enemy))
	//{

	//}
}
