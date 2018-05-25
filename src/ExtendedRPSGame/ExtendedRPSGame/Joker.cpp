#include "stdafx.h"
#include "Joker.h"
#include "Piece.h"
#include "Player.h"

bool Joker::SetActualPiece(unique_ptr<Piece> newPiece)
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
		}
		
		newPiece->GetOwner()->AddToCountMovingPieces(newPieceType);
	}

	mActualPiece = std::move(newPiece);
	return true;
}

Piece::WinningPiece Joker::FightPieceOfTheSameType(Piece& enemy)
{
	// Assert enemy is Joker? This method should not be called with other types.
	Piece::WinningPiece winningPiece = mActualPiece->Fight(enemy);
	if (winningPiece == Piece::WinningPiece::CallingObject)
	{
		enemy.LoseToPiece();
	}
	else if (winningPiece == Piece::WinningPiece::OtherObject)
	{
		this->LoseToPiece();
	}

	return winningPiece;
}

Piece::WinningPiece Joker::FightWithOtherPieceType(Piece& enemy)
{
	Piece::WinningPiece winningPiece = mActualPiece->Fight(enemy);

	if (winningPiece == Piece::WinningPiece::CallingObject)
	{
		enemy.LoseToPiece();
	}
	else if (winningPiece == Piece::WinningPiece::OtherObject)
	{
		this->LoseToPiece();
	}

	return winningPiece;
}
