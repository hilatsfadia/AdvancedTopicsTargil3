#include "stdafx.h"
#include "Joker.h"
#include "Piece.h"
#include "Player.h"

bool Joker::SetActualPieceType(Piece* newPiece)
{
	PieceFactory::PieceType newPieceType = newPiece->GetPieceType();
	if ((newPieceType != PieceFactory::PieceType::Rock) &&
		(newPieceType != PieceFactory::PieceType::Paper) &&
		(newPieceType != PieceFactory::PieceType::Scissors) &&
		(newPieceType != PieceFactory::PieceType::Bomb))
	{
		return false;
	}

	if (this->GetOwner() != nullptr)
	{
		if (this->GetOwner() != newPiece->GetOwner())
		{
			// Shouldn't happen.
			return false;
		}

		// newPiece is ok
		if (mActualPiece != nullptr)
		{
			mActualPiece->GetOwner()->RemoveFromCountMovingPieces(mActualPiece->GetPieceType());
			delete mActualPiece;
		}
		
		newPiece->GetOwner()->AddToCountMovingPieces(newPieceType);
	}

	mActualPiece = newPiece;
	return true;
}

Piece* Joker::FightPieceOfTheSameType(Piece* enemy)
{
	//if (Joker* enemyJoker = dynamic_cast<Joker*>(enemy))
	//{
	//}

	// Assert this is the case. This method should not be called with other types.
	Joker* enemyJoker = dynamic_cast<Joker*>(enemy);
	Piece* winningPiece = mActualPiece->Fight(enemyJoker->mActualPiece);
	if (winningPiece == nullptr)
	{
		return nullptr;
	}
	else if (winningPiece == mActualPiece)
	{
		return enemyJoker->LoseToPiece(this);
	}
	else if (winningPiece == enemyJoker->mActualPiece)
	{
		return this->LoseToPiece(enemyJoker);
	}

	// Assert in this case that winningPiece == nullptr
	return nullptr;
}

Piece* Joker::FightWithOtherPieceType(Piece* enemy)
{
	Piece* winningPiece = mActualPiece->Fight(enemy);

	if (winningPiece == nullptr)
	{
		return nullptr;
	}
	else if (winningPiece == mActualPiece)
	{
		return this;
	}
	else if (winningPiece == enemy)
	{
		return this->LoseToPiece(enemy);
	}

	// Assert in this case that winningPiece == nullptr
	return nullptr;
}
