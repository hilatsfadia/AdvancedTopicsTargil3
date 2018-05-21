#include "stdafx.h"
#include "Player.h"

std::map<PieceFactory::PieceType, int> Player::mMaxCountPieces;
int Player::mPlayerCounter = 1;

Player::~Player()
{
}

Player::Player()
{
	mPlayerNum = mPlayerCounter;
	mPlayerCounter++;
	mCountPieces[PieceFactory::PieceType::Rock] = 0;
	mCountPieces[PieceFactory::PieceType::Paper] = 0;
	mCountPieces[PieceFactory::PieceType::Scissors] = 0;
	mCountPieces[PieceFactory::PieceType::Bomb] = 0;
	mCountPieces[PieceFactory::PieceType::Joker] = 0;
	mCountPieces[PieceFactory::PieceType::Flag] = 0;

	mMaxCountPieces[PieceFactory::PieceType::Rock] = R;
	mMaxCountPieces[PieceFactory::PieceType::Paper] = P;
	mMaxCountPieces[PieceFactory::PieceType::Scissors] = S;
	mMaxCountPieces[PieceFactory::PieceType::Bomb] = B;
	mMaxCountPieces[PieceFactory::PieceType::Joker] = J;
	mMaxCountPieces[PieceFactory::PieceType::Flag] = F;
}

//
//Player::Player(PlayerAlgorithm* playerAlgorithm)
//{
//	mPlayerNum = mPlayerCounter;
//	mPlayerCounter++;
//	mPlayerAlgorithm = playerAlgorithm;
//	mCountPieces[PieceFactory::PieceType::Rock] = 0;
//	mCountPieces[PieceFactory::PieceType::Paper] = 0;
//	mCountPieces[PieceFactory::PieceType::Scissors] = 0;
//	mCountPieces[PieceFactory::PieceType::Bomb] = 0;
//	mCountPieces[PieceFactory::PieceType::Joker] = 0;
//	mCountPieces[PieceFactory::PieceType::Flag] = 0;
//
//	mMaxCountPieces[PieceFactory::PieceType::Rock] = R;
//	mMaxCountPieces[PieceFactory::PieceType::Paper] = P;
//	mMaxCountPieces[PieceFactory::PieceType::Scissors] = S;
//	mMaxCountPieces[PieceFactory::PieceType::Bomb] = B;
//	mMaxCountPieces[PieceFactory::PieceType::Joker] = J;
//	mMaxCountPieces[PieceFactory::PieceType::Flag] = F;
//}

int Player::GetPieceCount(PieceFactory::PieceType pieceType)
{
	return mCountPieces[pieceType];
}

bool Player::DecPieceCount(PieceFactory::PieceType pieceType)
{
	if (mCountPieces[pieceType] == 0)
	{
		return false;
	}

	mCountPieces[pieceType] = mCountPieces[pieceType] - 1;

	RemoveFromCountMovingPieces(pieceType);

	return true;
}

bool Player::IncPieceCountInInitialization(PieceFactory::PieceType pieceType)
{
	if (mCountPieces[pieceType] == mMaxCountPieces[pieceType])
	{
		return false;
	}

	mCountPieces[pieceType] = mCountPieces[pieceType] + 1;

	AddToCountMovingPieces(pieceType);

	if (pieceType == PieceFactory::PieceType::Flag)
	{
		mPositionedAllFlags = (mCountPieces[pieceType] == mMaxCountPieces[pieceType]);
	}

	return true;
}

int Player::GetCountOfMovingPieces()
{
	return mCountMovingPieces;
}

void Player::AddToCountMovingPieces(PieceFactory::PieceType pieceType)
{
	if ((pieceType == PieceFactory::PieceType::Rock) ||
		(pieceType == PieceFactory::PieceType::Paper) ||
		(pieceType == PieceFactory::PieceType::Scissors))
	{
		mCountMovingPieces++;
	}
}

void Player::RemoveFromCountMovingPieces(PieceFactory::PieceType pieceType)
{
	if ((pieceType == PieceFactory::PieceType::Rock) ||
		(pieceType == PieceFactory::PieceType::Paper) ||
		(pieceType == PieceFactory::PieceType::Scissors))
	{
		mCountMovingPieces--;
	}
}

//
//bool operator==(const Player& lhs, const Player& rhs)
//{
//	return lhs.mPlayerNum == rhs.mPlayerNum;
//}
