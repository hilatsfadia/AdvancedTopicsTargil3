#include "Piece.h"
#include "Player.h"
#include <cctype>

using namespace HilaAndJaelExtendedRPS;

void Piece::LoseToPiece()
{
	// TODO: throw exception. maybe move to destructor
	if (!this->mOwner->DecPieceCount(this->GetPieceType()))
	{
	}
}

void Piece::BothPiecesLosers(Piece& enemy)
{
	PieceType enemyPieceType = enemy.GetPieceType();

	// TODO: throw exception
	if (!this->mOwner->DecPieceCount(this->GetPieceType()))
	{
	}

	// TODO: throw exception
	if (!enemy.mOwner->DecPieceCount(enemyPieceType))
	{
	}
}

bool Piece::InitializeOwner(std::shared_ptr<Player> owner)
{ 
	mOwnerNum = owner->GetPlayerNum();
	PieceType pieceType = GetPieceType();
	// A PIECE type appears in file more than its number
	if (!owner->IncPieceCountInInitialization(pieceType))
	{
		return false;
	}

	mOwner = owner;

	return true;
}

Piece::WinningPiece Piece::FightPieceOfTheSameType(Piece& enemy)
{
	BothPiecesLosers(enemy);
	return Piece::WinningPiece::Tie;
}

Piece::WinningPiece Piece::Fight(Piece& enemy)
{
	if (this->GetPieceType() == enemy.GetPieceType())
	{
		// Fight between tools of the same type
		return FightPieceOfTheSameType(enemy);
	}

	return FightWithOtherPieceType(enemy);
}

namespace HilaAndJaelExtendedRPS
{
	// Operator overloading for printing issues.
	std::ostream& operator<<(std::ostream& out, const Piece& piece) {
		//Player 1: all chars as capital letters
		//Player 2: all chars as lower letters

		if (piece.GetOwnerNum() == 2)
		{
			return out << static_cast<char>(std::tolower(static_cast<unsigned char>(piece.GetPieceChar())));
		}

		return out << piece.GetPieceChar();
	}
}
