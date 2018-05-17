#include "stdafx.h"
#include "StrategyPiece.h"

PieceFactory::PieceType StrategyPiece::GetPieceType() const
{
	return ((mUncoveredPiece == nullptr) ? PieceFactory::PieceType::Unknown : mUncoveredPiece->GetPieceType());
}

bool StrategyPiece::isMovingPiece() const
{
	return ((mUncoveredPiece == nullptr) ? false : mUncoveredPiece->isMovingPiece());
}

char StrategyPiece::GetPieceChar() const
{
	return ((mUncoveredPiece == nullptr) ? COVERED_CHAR : mUncoveredPiece->GetPieceChar());
}

void StrategyPiece::UncoverPiece(char uncoveredPieceChar)
{
	mUncoveredPiece = PieceFactory::GetPieceFromChar(uncoveredPieceChar, mOwnerNum);
}
