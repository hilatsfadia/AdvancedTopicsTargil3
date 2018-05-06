#include "stdafx.h"
#include "Piece.h"
#include "Player.h"
#include <cctype>

Piece* Piece::LoseToPiece(Piece* enemy)
{
	// TODO: throw exception. maybe move to destructor
	if (!this->mOwner->DecPieceCount(this->GetPieceType()))
	{
	}

	// TODO: delete for move files
	// As long as you're careful, its okay (not evil) for an object to commit suicide (delete this).
	//delete this;
	return enemy;
}

//
//Piece* Piece::WinPiece(Piece* enemy, PieceFactory::PieceType enemyPieceType)
//{
//	// TODO: throw exception
//	if (!this->mOwner.DecPieceCount(enemyPieceType))
//	{
//	}
//
//	delete enemy;
//	return this;
//}

Piece* Piece::BothPiecesLosers(Piece* enemy)
{
	PieceFactory::PieceType enemyPieceType = enemy->GetPieceType();

	// TODO: throw exception
	if (!this->mOwner->DecPieceCount(this->GetPieceType()))
	{
	}

	// TODO: throw exception
	if (!enemy->mOwner->DecPieceCount(enemyPieceType))
	{
	}

	// TODO: delete for move files
	//delete enemy;
	// As long as you're careful, its okay (not evil) for an object to commit suicide (delete this).
	//delete this;
	return nullptr;
}

bool Piece::InitializeOwner(Player* owner)
{ 
	PieceFactory::PieceType pieceType = GetPieceType();
	// A PIECE type appears in file more than its number
	if (!owner->IncPieceCountInInitialization(pieceType))
	{
		return false;
	}

	SetTransparentOwner(owner);

	return true;
}

void Piece::SetTransparentOwner(Player * owner)
{
	mOwner = owner;

	//Player 1: all chars as capital letters
	//Player 2: all chars as lower letters
	if (owner->GetPlayerNum() == 1)
	{
		mPieceChar = toupper(mPieceChar);
	}
	else if (owner->GetPlayerNum() == 2)
	{
		mPieceChar = tolower(mPieceChar);
	}
}

Piece* Piece::FightPieceOfTheSameType(Piece* enemy)
{
	// Fight between tools of the same type
	return BothPiecesLosers(enemy);
}

Piece* Piece::Fight(Piece* enemy)
{
	if (enemy == nullptr)
	{
		return this;
	}
	
	PieceFactory::PieceType enemyPieceType = enemy->GetPieceType();

	if (this->GetPieceType() == enemyPieceType)
	{
		// Fight between tools of the same type
		return FightPieceOfTheSameType(enemy);
	}

	return FightWithOtherPieceType(enemy);
}
