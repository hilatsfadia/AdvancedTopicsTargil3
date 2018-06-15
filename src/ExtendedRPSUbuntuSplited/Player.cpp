#include "Player.h"

using namespace HilaAndJaelExtendedRPS;

Player::~Player()
{
}

Player::Player(int playerNum)
{
	mPlayerNum = playerNum;
	mCountPieces[PieceType::Rock] = 0;
	mCountPieces[PieceType::Paper] = 0;
	mCountPieces[PieceType::Scissors] = 0;
	mCountPieces[PieceType::Bomb] = 0;
	mCountPieces[PieceType::Joker] = 0;
	mCountPieces[PieceType::Flag] = 0;

	mMaxCountPieces[PieceType::Rock] = R;
	mMaxCountPieces[PieceType::Paper] = P;
	mMaxCountPieces[PieceType::Scissors] = S;
	mMaxCountPieces[PieceType::Bomb] = B;
	mMaxCountPieces[PieceType::Joker] = J;
	mMaxCountPieces[PieceType::Flag] = F;
}

//
//Player::Player(PlayerAlgorithm* playerAlgorithm)
//{
//	mPlayerNum = mPlayerCounter;
//	mPlayerCounter++;
//	mPlayerAlgorithm = playerAlgorithm;
//	mCountPieces[PieceType::Rock] = 0;
//	mCountPieces[PieceType::Paper] = 0;
//	mCountPieces[PieceType::Scissors] = 0;
//	mCountPieces[PieceType::Bomb] = 0;
//	mCountPieces[PieceType::Joker] = 0;
//	mCountPieces[PieceType::Flag] = 0;
//
//	mMaxCountPieces[PieceType::Rock] = R;
//	mMaxCountPieces[PieceType::Paper] = P;
//	mMaxCountPieces[PieceType::Scissors] = S;
//	mMaxCountPieces[PieceType::Bomb] = B;
//	mMaxCountPieces[PieceType::Joker] = J;
//	mMaxCountPieces[PieceType::Flag] = F;
//}

int Player::GetPieceCount(PieceType pieceType) const
{
	auto pieceCount = mCountPieces.find(pieceType);

	if (pieceCount != mCountPieces.end()) {
		return pieceCount->second;
	}

	return 0;
}

bool Player::DecPieceCount(PieceType pieceType)
{
	if (mCountPieces[pieceType] == 0)
	{
		return false;
	}

	mCountPieces[pieceType] = mCountPieces[pieceType] - 1;

	RemoveFromCountMovingPieces(pieceType);

	return true;
}

bool Player::IncPieceCountInInitialization(PieceType pieceType)
{
	if (mCountPieces[pieceType] == mMaxCountPieces[pieceType])
	{
		return false;
	}

	mCountPieces[pieceType] = mCountPieces[pieceType] + 1;

	AddToCountMovingPieces(pieceType);

	if (pieceType == PieceType::Flag)
	{
		mPositionedAllFlags = (mCountPieces[pieceType] == mMaxCountPieces[pieceType]);
	}

	return true;
}

int Player::GetCountOfMovingPieces() const
{
	return mCountMovingPieces;
}
//
//int Player::GetFlagsCount() const
//{
//	auto flagsCount = mCountPieces.find(PieceType::Flag);
//
//	if (flagsCount != mCountPieces.end()) {
//		return flagsCount->second;
//	}
//
//	return 0;
//}

void Player::AddToCountMovingPieces(PieceType pieceType)
{
	if ((pieceType == PieceType::Rock) ||
		(pieceType == PieceType::Paper) ||
		(pieceType == PieceType::Scissors))
	{
		mCountMovingPieces++;
	}
}

void Player::RemoveFromCountMovingPieces(PieceType pieceType)
{
	if ((pieceType == PieceType::Rock) ||
		(pieceType == PieceType::Paper) ||
		(pieceType == PieceType::Scissors))
	{
		mCountMovingPieces--;
	}
}

//
//bool operator==(const Player& lhs, const Player& rhs)
//{
//	return lhs.mPlayerNum == rhs.mPlayerNum;
//}
