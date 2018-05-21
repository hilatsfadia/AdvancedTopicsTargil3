#include "stdafx.h"
#include "StrategyPiece.h"
#include "Joker.h"
#include "Rock.h"
#include "Paper.h"
#include "Scissors.h"
#include "Joker.h"

PieceFactory::PieceType StrategyPiece::GetPieceType() const
{
	return ((mUncoveredPiece == nullptr) ? PieceFactory::PieceType::Unknown : mUncoveredPiece->GetPieceType());
}

bool StrategyPiece::GetIsMovingPiece() const
{
	if (mUncoveredPiece != nullptr)
	{
		return mUncoveredPiece->GetIsMovingPiece();
	}

	return mIsMovingPiece;
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

char StrategyPiece::GetStrongerNotJoker(const Piece * piece) const
{
	PieceFactory::PieceType pieceType = piece->GetPieceType();

	switch (pieceType)
	{
		case (PieceFactory::PieceType::Paper):
		{
			return SCISSORS_CHAR;
			break;
		}
		case (PieceFactory::PieceType::Scissors):
		{
			return ROCK_CHAR;
			break;
		}
		case (PieceFactory::PieceType::Rock):
		{
			return PAPER_CHAR;
			break;
		}
		default:
		{
			return COVERED_CHAR;
		}
	}
}

char StrategyPiece::GetStronger(const Piece* piece) const
{
	PieceFactory::PieceType type = piece->GetPieceType();

	switch (type)
	{
		case (PieceFactory::PieceType::Joker):
		{
			const Joker* joker = dynamic_cast<const Joker*>(piece);
			if (joker != nullptr)
			{
				return GetStrongerNotJoker(joker->GetActualPiece());
			}

			break;
		}
		default:
		{
			return GetStrongerNotJoker(piece);
			break;
		}
	}

	return COVERED_CHAR;
}
