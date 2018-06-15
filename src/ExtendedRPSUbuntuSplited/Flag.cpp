#include "Flag.h"

using namespace HilaAndJaelExtendedRPS;

Piece::WinningPiece Flag::FightWithOtherPieceType(Piece&)
{
	LoseToPiece();
	return WinningPiece::OtherObject;
}
