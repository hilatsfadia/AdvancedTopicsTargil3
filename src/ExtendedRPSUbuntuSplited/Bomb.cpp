#include "Bomb.h"

using namespace HilaAndJaelExtendedRPS;

Piece::WinningPiece Bomb::FightWithOtherPieceType(Piece& enemy)
{
	BothPiecesLosers(enemy);
	return WinningPiece::Tie;
}
