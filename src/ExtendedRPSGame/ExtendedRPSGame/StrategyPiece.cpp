#include "stdafx.h"
#include "StrategyPiece.h"
#include "Joker.h"

PieceFactory::PieceType StrategyPiece::GetPieceType() const
{
	return ((mUncoveredPiece == nullptr) ? PieceFactory::PieceType::Unknown : mUncoveredPiece->GetPieceType());
}

char StrategyPiece::GetPieceChar() const
{
	return ((mUncoveredPiece == nullptr) ? COVERED_CHAR : mUncoveredPiece->GetPieceChar());
}

void StrategyPiece::UncoverPiece(Piece* uncoveredPiece)
{
	mUncoveredPiece = uncoveredPiece;

	if (mUncoveredPiece != nullptr)
	{
		mIsMovingPiece = mUncoveredPiece->GetIsMovingPiece();
	}
}

void StrategyPiece::UncoverPiece(char uncoveredPieceChar)
{
	UncoverPiece(PieceFactory::GetPieceFromChar(uncoveredPieceChar, mOwnerNum));
}

bool StrategyPiece::IsStrongerThan(Piece* other) const
{
	if (mUncoveredPiece != nullptr)
	{
		return mUncoveredPiece->IsStrongerThan(other);
	}

	return false;
}

PieceFactory::PieceType StrategyPiece::GetStronger(const Piece* piece) const
{
	PieceFactory::PieceType pieceType = piece->GetPieceType();

	switch (pieceType)
	{
		case (PieceFactory::PieceType::Paper):
		{
			return PieceFactory::PieceType::Scissors;
			break;
		}
		case (PieceFactory::PieceType::Scissors):
		{
			return PieceFactory::PieceType::Rock;
			break;
		}
		case (PieceFactory::PieceType::Rock):
		{
			return PieceFactory::PieceType::Paper;
			break;
		}
		case (PieceFactory::PieceType::Joker):
		{
			const Joker* joker = dynamic_cast<const Joker*>(piece);
			return GetStronger(joker->GetActualPiece());
			break;
		}
		default:
		{
			return PieceFactory::PieceType::Unknown;
			break;
		}
	}

	return PieceFactory::PieceType::Unknown;
}
