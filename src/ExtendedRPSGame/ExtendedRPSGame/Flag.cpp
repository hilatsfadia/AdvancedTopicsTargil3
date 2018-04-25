#include "stdafx.h"
#include "Flag.h"

Piece* Flag::FightWithOtherPieceType(Piece* enemy)
{
	return LoseToPiece(enemy);
}
