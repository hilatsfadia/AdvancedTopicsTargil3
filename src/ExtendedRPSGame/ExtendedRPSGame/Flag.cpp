#include "Flag.h"

Piece::WinningPiece Flag::FightWithOtherPieceType(Piece&)
{
	LoseToPiece();
	return WinningPiece::OtherObject;
}
