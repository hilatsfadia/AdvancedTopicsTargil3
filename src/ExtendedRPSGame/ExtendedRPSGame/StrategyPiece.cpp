#include "stdafx.h"
#include "StrategyPiece.h"
#include "Joker.h"
#include "Rock.h"
#include "Paper.h"
#include "Scissors.h"
#include "Joker.h"
#include <memory>

int StrategyPiece::mStrategyPiecesCounter = 1;

StrategyPiece::StrategyPiece(int ownerNum, unique_ptr<Piece> uncoveredPiece) : Piece(ownerNum)
{
	if (uncoveredPiece != nullptr)
	{
		UncoverPiece(std::move(uncoveredPiece));
	}

	mStrategyPieceID = mStrategyPiecesCounter;
	mStrategyPiecesCounter++;
}

PieceFactory::PieceType StrategyPiece::GetPieceType() const
{
	return ((mUncoveredPiece == nullptr) ? PieceFactory::PieceType::Covered : mUncoveredPiece->GetPieceType());
}

PieceFactory::PieceType StrategyPiece::GetActualPieceType() const
{
	return ((mUncoveredPiece == nullptr) ? PieceFactory::PieceType::Covered : mUncoveredPiece->GetActualPieceType());
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
			if (mUncoveredPiece->GetPieceType() == PieceFactory::PieceType::Joker)
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

void StrategyPiece::ChangeJokerToStronger(PieceFactory::PieceType enemyPieceType)
{
	if (this->GetPieceType() == PieceFactory::PieceType::Joker)
	{
		switch (enemyPieceType)
		{
			case (PieceFactory::PieceType::Paper):
			{
				mUncoveredPiece = std::make_unique<Joker>(mOwnerNum, std::make_unique<Scissors>(mOwnerNum));
				break;
			}
			case (PieceFactory::PieceType::Scissors):
			{
				mUncoveredPiece = std::make_unique<Joker>(mOwnerNum, std::make_unique<Rock>(mOwnerNum));
				break;
			}
			case (PieceFactory::PieceType::Rock):
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

//
//char StrategyPiece::GetStronger(const Piece* piece) const
//{
//	PieceFactory::PieceType type = piece->GetPieceType();
//
//	switch (type)
//	{
//		case (PieceFactory::PieceType::Joker):
//		{
//			const Joker* joker = dynamic_cast<const Joker*>(piece);
//			if (joker != nullptr)
//			{
//				return GetStrongerNotJoker(joker->GetActualPiece());
//			}
//
//			break;
//		}
//		default:
//		{
//			return GetStrongerNotJoker(piece);
//			break;
//		}
//	}
//
//	return COVERED_CHAR;
//}
