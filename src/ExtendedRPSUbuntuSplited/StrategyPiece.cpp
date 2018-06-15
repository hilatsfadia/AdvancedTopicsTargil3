#include "StrategyPiece.h"
#include "Joker.h"
#include "Rock.h"
#include "Paper.h"
#include "Scissors.h"
#include "Joker.h"
#include <memory>

using namespace HilaAndJaelExtendedRPS;

PieceType StrategyPiece::GetPieceType() const
{
	return ((mUncoveredPiece == nullptr) ? PieceType::Covered : mUncoveredPiece->GetPieceType());
}

PieceType StrategyPiece::GetActualPieceType() const
{
	return ((mUncoveredPiece == nullptr) ? PieceType::Covered : mUncoveredPiece->GetActualPieceType());
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

char StrategyPiece::GetActualPieceChar() const
{
	return ((mUncoveredPiece == nullptr) ? COVERED_CHAR : mUncoveredPiece->GetActualPieceChar());
}

void StrategyPiece::UncoverPiece(std::unique_ptr<Piece> uncoveredPiece)
{
	if (uncoveredPiece != nullptr)
	{
		mUncoveredPiece = std::move(uncoveredPiece);
		mIsMovingPiece = mUncoveredPiece->GetIsMovingPiece();
	}
}

void StrategyPiece::UncoverPiece(char uncoveredPieceChar)
{
	if (mUncoveredPiece != nullptr)
	{
		if (mUncoveredPiece->GetPieceChar() != uncoveredPieceChar) // If this piece should be a joker
		{
			// TODO: maybe not needed
			if (mUncoveredPiece->GetPieceType() == PieceType::Joker)
			{
				Joker& joker = dynamic_cast<Joker&>(*mUncoveredPiece);
				if (joker.PeekActualPiece().GetPieceChar() != uncoveredPieceChar)
				{
					joker.SetActualPiece(PieceFactory::GetPieceFromChar(uncoveredPieceChar, mOwnerNum));
					mIsMovingPiece = mUncoveredPiece->GetIsMovingPiece();
				}
			}
			else
			{
				UncoverPiece(std::make_unique<Joker>(mOwnerNum, PieceFactory::GetPieceFromChar(uncoveredPieceChar, mOwnerNum)));
			}
		}
	}
	else
	{
		UncoverPiece(PieceFactory::GetPieceFromChar(uncoveredPieceChar, mOwnerNum));
	}
}

bool StrategyPiece::IsStrongerThan(const StrategyPiece& other) const
{
	if ((mUncoveredPiece != nullptr) && (other.mUncoveredPiece != nullptr))
	{
		return mUncoveredPiece->IsStrongerThan(*other.mUncoveredPiece);
	}

	return false;
}

void StrategyPiece::ChangeJokerToStronger(PieceType enemyPieceType)
{
	if (this->GetPieceType() == PieceType::Joker)
	{
		switch (enemyPieceType)
		{
			case (PieceType::Paper):
			{
				mUncoveredPiece = std::make_unique<Joker>(mOwnerNum, std::make_unique<Scissors>(mOwnerNum));
				break;
			}
			case (PieceType::Scissors):
			{
				mUncoveredPiece = std::make_unique<Joker>(mOwnerNum, std::make_unique<Rock>(mOwnerNum));
				break;
			}
			case (PieceType::Rock):
			{
				mUncoveredPiece = std::make_unique<Joker>(mOwnerNum, std::make_unique<Paper>(mOwnerNum));
				break;
			}
			default:
			{
				break;
			}
		}
	}
}
