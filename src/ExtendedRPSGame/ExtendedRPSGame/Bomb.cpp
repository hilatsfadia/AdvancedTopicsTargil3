#include "stdafx.h"
#include "Bomb.h"

Piece::WinningPiece Bomb::FightWithOtherPieceType(Piece& enemy)
{
	BothPiecesLosers(enemy);
	return WinningPiece::Tie;
}
