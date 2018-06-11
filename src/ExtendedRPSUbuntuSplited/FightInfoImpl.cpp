#include "FightInfoImpl.h"

void FightInfoImpl::SetFightInfoValues(const Point& position, char player1Piece, char player2Piece, int winner)
{
	mPos = std::make_unique<PointImpl>(position.getX(), position.getY());
	mPlayersPieces[0] = player1Piece;
	mPlayersPieces[1] = player2Piece;
	mWinner = winner;
}
