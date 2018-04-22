#include "stdafx.h"
#include "Bomb.h"

Piece* Bomb::FightWithOtherPieceType(Piece* enemy)
{
	return BothPiecesLosers(enemy);
}
