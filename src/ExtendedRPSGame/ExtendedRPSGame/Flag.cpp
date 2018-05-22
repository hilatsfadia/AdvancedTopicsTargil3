#include "stdafx.h"
#include "Flag.h"

Piece::WinningPiece Flag::FightWithOtherPieceType(Piece& enemy)
{
	LoseToPiece();
	return WinningPiece::OtherObject;
}
